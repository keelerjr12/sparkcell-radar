#ifndef RADAR_GAUGE_DRAWABLE_H
#define RADAR_GAUGE_DRAWABLE_H

#include <gauges.h>
#include <gdiplus.h>
#include <memory>
#include <string>

class VirtualDisplay {

 public: 

    VirtualDisplay(HDC hdc,  int x_sz, int y_sz) : 
		m_graphics(hdc),
		m_x_sz(x_sz), 
		m_y_sz(y_sz), 
		m_fnt(std::make_unique<Gdiplus::Font>(L"Times New Roman", 1.0)),
		m_pen(std::make_unique<Gdiplus::Pen>(Gdiplus::Color::White, 1.f)) { }

	void Clear() {
		m_graphics.Clear(Gdiplus::Color::Transparent);
	}

    void DrawString(std::wstring txt, float x_pos, float y_pos) {
        Gdiplus::PointF pt{ x_pos * m_x_sz, y_pos * m_y_sz };
	    Gdiplus::SolidBrush  solidBrush(Gdiplus::Color(255, 255, 255, 255));

        m_graphics.DrawString(txt.c_str(), -1, m_fnt.get(), pt, &solidBrush);
    }

	void DrawLine(float x1, float y1, float x2, float y2) {
		const auto x1_px = m_x_sz * x1;
		const auto y1_px = m_y_sz * y1;
		const auto x2_px = m_x_sz * x2;
		const auto y2_px = m_y_sz * y2;

		m_graphics.DrawLine(m_pen.get(), Gdiplus::Point(x1_px, y1_px), Gdiplus::Point(x2_px, y2_px));
	}

	void DrawRect(float x, float y, float width, float height) {
		const auto x_px = m_x_sz * x;
		const auto y_px = m_y_sz * y;
		const auto w_px = m_x_sz * width;
		const auto h_px = m_y_sz * height;

	    Gdiplus::SolidBrush brush(Gdiplus::Color(255, 255, 255, 255));
		m_graphics.FillRectangle(&brush, x_px, y_px, w_px, h_px);
	}

    void SetFontSize(float fnt_sz) {
		m_pvc.AddFontFile(L"C:\\Users\\ADMIN\\Fonts\\B612\\B612-Regular.ttf");
        m_fnt = std::make_unique<Gdiplus::Font>(L"B612", fnt_sz, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel, &m_pvc);
    }

 private:

	Gdiplus::Graphics m_graphics;

    int m_x_sz;
    int m_y_sz;

    Gdiplus::PrivateFontCollection m_pvc;
    std::unique_ptr<Gdiplus::Font> m_fnt;
	std::unique_ptr<Gdiplus::Pen> m_pen;
};

namespace SparkCell {
	class Radar;
}

enum RadarGaugeDrawableId {
	RADAR
};

class RadarGaugeDrawable : public IGaugeCDrawable {
public:
	RadarGaugeDrawable(const IGaugeCDrawableCreateParameters* pParams, const SparkCell::Radar* const radar);

	// Inherited via IGaugeCDrawable
	virtual ULONG AddRef() override;
	virtual ULONG Release() override;
	virtual FLAGS32 GetFlags() override;
	virtual void Update() override;
	virtual void Show(bool on) override;
	virtual bool Draw(IGaugeCDrawableDrawParameters* pParameters, PIXPOINT size, HDC hdc, PIMAGE pImage) override;
	virtual bool SetupDraw(PIXPOINT size, HDC hdc, PIMAGE pImage) override;
	virtual bool GetDraw(IGaugeCDrawableDrawParameters* pParameters) override;

private:
	const SparkCell::Radar* const mRadar;
	std::unique_ptr<VirtualDisplay> vd;
};

#endif // RADAR_GAUGE_DRAWABLE_H
