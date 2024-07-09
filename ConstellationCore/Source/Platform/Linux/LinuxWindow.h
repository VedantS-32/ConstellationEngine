#pragma once

#ifdef CSTELL_PLATFORM_LINUX

	#include "CStell/Core/Window.h"
	#include "CStell/Renderer/RendererContext.h"
	#include "Platform/OpenGL/OpenGLContext.h"

	#include <GLFW/glfw3.h>

	namespace CStell
	{
		class CSTELL_API LinuxWindow : public Window
		{
		public:
			LinuxWindow(const WindowProps& props);
			virtual ~LinuxWindow();

			void OnUpdate() override;

			inline unsigned int GetWidth() const override { return m_Data.Width; }
			inline unsigned int GetHeight() const override { return m_Data.Height; }

			// Window attributes
			virtual void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
			virtual void SetVSync(bool enabled) override;
			virtual bool IsVSync() const override;

			inline virtual void* GetNativeWindow() const { return m_Window; }

		private:
			virtual void Init(const WindowProps& props);
			virtual void Shutdown();

		private:
			GLFWwindow* m_Window;
			RendererContext* m_Context;

			struct WindowData
			{
				std::string Title;
				unsigned int Width, Height;
				bool VSync = true;

				EventCallbackFn EventCallback;
			};

			WindowData m_Data;
		};
	}

#endif