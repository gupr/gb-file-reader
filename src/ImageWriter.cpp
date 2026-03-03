#include "ImageWriter.h"
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <cstring>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

// ===============================
// Configuration
// ===============================

static constexpr int FONT_SIZE = 64;

static int GetFooterHeight()
{
    return FONT_SIZE + 40;
}

static constexpr const char *FONT_PATH = "font.ttf";

// Font Cache (loaded once)
static stbtt_fontinfo g_font;
static std::vector<uint8_t> g_fontBuffer;
static bool g_fontLoaded = false;

static void EnsureFontLoaded()
{
    if (g_fontLoaded)
        return;

    std::ifstream file(FONT_PATH, std::ios::binary);
    if (!file)
        throw std::runtime_error("Failed to open font file.");

    g_fontBuffer = std::vector<uint8_t>(
        std::istreambuf_iterator<char>(file),
        {});

    if (!stbtt_InitFont(
            &g_font,
            g_fontBuffer.data(),
            stbtt_GetFontOffsetForIndex(
                g_fontBuffer.data(), 0)))
    {
        throw std::runtime_error("Failed to initialize font.");
    }

    g_fontLoaded = true;
}

// Public API
void ImageWriter::WritePNG(
    const std::string &path,
    const PixelBuffer &pixels,
    int width,
    int height,
    const std::string &title,
    size_t romSizeBytes)
{
    if (pixels.empty() || width <= 0 || height <= 0)
        throw std::runtime_error("Invalid image data.");

    const int finalHeight = height + GetFooterHeight();

    // Build RGB buffer (with footer space)
    std::vector<uint8_t> rgb(width * finalHeight * 3);

    // Copy original image
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            const auto &p = pixels[y * width + x];
            int i = (y * width + x) * 3;

            rgb[i + 0] = p.r;
            rgb[i + 1] = p.g;
            rgb[i + 2] = p.b;
        }
    }

    // Fill footer background (dark gray)
    for (int y = height; y < finalHeight; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            int i = (y * width + x) * 3;
            rgb[i + 0] = 35;
            rgb[i + 1] = 35;
            rgb[i + 2] = 35;
        }
    }

    // Divider line
    for (int x = 0; x < width; ++x)
    {
        int i = (height * width + x) * 3;
        rgb[i + 0] = 200;
        rgb[i + 1] = 200;
        rgb[i + 2] = 200;
    }

    // Compose footer text
    std::ostringstream oss;
    oss << title << "  |  "
        << (romSizeBytes / 1024) << " KB";

    RenderFooter(
        rgb,
        width,
        height,
        GetFooterHeight(),
        oss.str());

    // Save PNG
    if (!stbi_write_png(
            path.c_str(),
            width,
            finalHeight,
            3,
            rgb.data(),
            width * 3))
    {
        throw std::runtime_error("Failed to write PNG.");
    }
}

// Text Rendering
void ImageWriter::RenderFooter(
    std::vector<uint8_t> &rgb,
    int width,
    int height,
    int footerHeight,
    const std::string &text)
{
    EnsureFontLoaded();

    float scale = stbtt_ScaleForPixelHeight(
        &g_font, FONT_SIZE);

    int baseline = height + footerHeight / 2 + FONT_SIZE / 3;
    int x = 20;

    for (char c : text)
    {
        int ax, lsb;
        stbtt_GetCodepointHMetrics(
            &g_font, c, &ax, &lsb);

        int x1, y1, x2, y2;
        stbtt_GetCodepointBitmapBox(
            &g_font, c, scale, scale,
            &x1, &y1, &x2, &y2);

        int w = x2 - x1;
        int h = y2 - y1;

        if (w <= 0 || h <= 0)
        {
            x += int(ax * scale);
            continue;
        }

        std::vector<unsigned char> bitmap(w * h);

        stbtt_MakeCodepointBitmap(
            &g_font,
            bitmap.data(),
            w,
            h,
            w,
            scale,
            scale,
            c);

        for (int gy = 0; gy < h; ++gy)
        {
            for (int gx = 0; gx < w; ++gx)
            {
                int px = x + gx + x1;
                int py = baseline + gy + y1;

                if (px < 0 || px >= width)
                    continue;

                int alpha = bitmap[gy * w + gx];
                if (alpha == 0)
                    continue;

                int i = (py * width + px) * 3;

                // Blend white text
                rgb[i + 0] = (rgb[i + 0] * (255 - alpha) + 255 * alpha) / 255;
                rgb[i + 1] = (rgb[i + 1] * (255 - alpha) + 255 * alpha) / 255;
                rgb[i + 2] = (rgb[i + 2] * (255 - alpha) + 255 * alpha) / 255;
            }
        }

        x += int(ax * scale);
    }
}