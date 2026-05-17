#pragma once


namespace dae
{
	class Camera
	{
	public:



		float x{ 0.f };
		float y{ 0.f };

		float zoom{ 1.f };
		


		Camera(int w, int h) : m_ViewWidth(w), m_ViewHeight(h)
		{
			float zoomX = float(m_screenWidth) / float(m_ViewWidth);
			float zoomY = float(m_screenHeight) / float(m_ViewHeight);

			zoom = std::min(zoomX, zoomY);
		}

		void SetViewDimensions(int w, int h) 
		{
			m_ViewWidth = w;
			m_ViewHeight = h;

			float zoomX = float(m_screenWidth) / float(m_ViewWidth);
			float zoomY = float(m_screenHeight) / float(m_ViewHeight);

			zoom = std::min(zoomX, zoomY);
		}

	private:
		int m_screenWidth{ 1280 };
		int m_screenHeight{ 576 };
		int m_ViewWidth;
		int m_ViewHeight;
	};


}