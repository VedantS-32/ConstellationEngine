#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>

#include <mutex>
#include <thread>

namespace CStell
{
    struct ProfileResult
    {
        const std::string name;
        long long start, end;
        uint32_t threadID;
    };

    class Instrumentor
    {
        std::string m_sessionName = "None";
        std::ofstream m_outputStream;
        int m_profileCount = 0;
        std::mutex m_lock;
        bool m_activeSession = false;

        Instrumentor() { }

    public:

        static Instrumentor& Get()
        {
            static Instrumentor instance;
            return instance;
        }

        ~Instrumentor()
        {
            EndSession();
        }

        void BeginSession(const std::string& name, const std::string& filepath = "results.json")
        {
            if (m_activeSession) { EndSession(); }
            m_activeSession = true;
            m_outputStream.open(filepath);
            WriteHeader();
            m_sessionName = name;
        }

        void EndSession()
        {
            if (!m_activeSession) { return; }
            m_activeSession = false;
            WriteFooter();
            m_outputStream.close();
            m_profileCount = 0;
        }

        void WriteProfile(const ProfileResult& result)
        {
            std::lock_guard<std::mutex> lock(m_lock);

            if (m_profileCount++ > 0) { m_outputStream << ","; }

            std::string name = result.name;
            std::replace(name.begin(), name.end(), '"', '\'');

            m_outputStream << "{";
            m_outputStream << "\"cat\":\"function\",";
            m_outputStream << "\"dur\":" << (result.end - result.start) << ',';
            m_outputStream << "\"name\":\"" << name << "\",";
            m_outputStream << "\"ph\":\"X\",";
            m_outputStream << "\"pid\":0,";
            m_outputStream << "\"tid\":" << result.threadID << ",";
            m_outputStream << "\"ts\":" << result.start;
            m_outputStream << "}";
        }

        void WriteHeader()
        {
            m_outputStream << "{\"otherData\": {},\"traceEvents\":[";
        }

        void WriteFooter()
        {
            m_outputStream << "]}";
        }
    };

    class InstrumentationTimer
    {
        ProfileResult m_result;

        std::chrono::time_point<std::chrono::high_resolution_clock> m_startTimepoint;
        bool m_stopped;

    public:

        InstrumentationTimer(const std::string& name)
            : m_result({ name, 0, 0, 0 })
            , m_stopped(false)
        {
            m_startTimepoint = std::chrono::high_resolution_clock::now();
        }

        ~InstrumentationTimer()
        {
            if (!m_stopped) { Stop(); }
        }

        void Stop()
        {
            auto endTimepoint = std::chrono::high_resolution_clock::now();

            m_result.start = std::chrono::time_point_cast<std::chrono::microseconds>(m_startTimepoint).time_since_epoch().count();
            m_result.end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();
            m_result.threadID = (uint32_t)std::hash<std::thread::id>{}(std::this_thread::get_id());
            Instrumentor::Get().WriteProfile(m_result);

            m_stopped = true;
        }
    };
}

#define CSTELL_PROFILE 0

#if CSTELL_PROFILE
#define CSTELL_PROFILE_BEGIN_SESSION(name, filepath) ::CStell::Instrumentor::Get().BeginSession(name, filepath)
#define CSTELL_PROFILE_END_SESSION() ::CStell::Instrumentor::Get().EndSession()
#define CSTELL_PROFILE_SCOPE(name) ::CStell::InstrumentationTimer timer##__LINE__(name);
#define CSTELL_PROFILE_FUNCTION() CSTELL_PROFILE_SCOPE(__FUNCSIG__)
#else
#define CSTELL_PROFILE_BEGIN_SESSION(name, filepath)
#define CSTELL_PROFILE_END_SESSION()
#define CSTELL_PROFILE_SCOPE(name)
#define CSTELL_PROFILE_FUNCTION()
#endif
