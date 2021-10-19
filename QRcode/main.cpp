#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <lunasvg.h>
#include "stb_image_write.h"

#include <climits>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include "qrcodegen.hpp"

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <zxing/LuminanceSource.h>
// #include <zxing/common/Counted.h>
// #include <zxing/Reader.h>
// #include <zxing/ReaderException.h>
// #include <zxing/Exception.h>
// #include <zxing/aztec/AztecReader.h>
#include <zxing/common/GlobalHistogramBinarizer.h>
// #include <zxing/common/IllegalArgumentException.h>
// #include <zxing/DecodeHints.h>
// #include <zxing/BinaryBitmap.h>
// #include <zxing/DecodeHints.h>
// #include <zxing/datamatrix/DataMatrixReader.h>
// #include <zxing/MultiFormatReader.h>
// #include <zxing/pdf417/PDF417Reader.h>
#include <zxing/qrcode/QRCodeReader.h>
#include <MatSource.h>

#include <hv/hv.h>
#include <hv/requests.h>

using namespace hv;
using namespace zxing;
using namespace lunasvg;
using namespace cv;

using qrcodegen::QrCode;
using qrcodegen::QrSegment;
using std::uint8_t;

static std::string url = "";
static void printQr(const QrCode &qr)
{
	int border = 4;
	for (int y = -border; y < qr.getSize() + border; y++)
	{
		for (int x = -border; x < qr.getSize() + border; x++)
		{
			std::cout << (qr.getModule(x, y) ? "##" : "  ");
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}
static std::string toSvgString(const QrCode &qr, int border)
{
	if (border < 0)
		throw std::domain_error("Border must be non-negative");
	if (border > INT_MAX / 2 || border * 2 > INT_MAX - qr.getSize())
		throw std::overflow_error("Border too large");

	std::ostringstream sb;
	sb << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	sb << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n";
	sb << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" viewBox=\"0 0 ";
	sb << (qr.getSize() + border * 2) << " " << (qr.getSize() + border * 2) << "\" stroke=\"none\">\n";
	sb << "\t<rect width=\"100%\" height=\"100%\" fill=\"#FFFFFF\"/>\n";
	sb << "\t<path d=\"";
	for (int y = 0; y < qr.getSize(); y++)
	{
		for (int x = 0; x < qr.getSize(); x++)
		{
			if (qr.getModule(x, y))
			{
				if (x != 0 || y != 0)
					sb << " ";
				sb << "M" << (x + border) << "," << (y + border) << "h1v1h-1z";
			}
		}
	}
	sb << "\" fill=\"#000000\"/>\n";
	sb << "</svg>\n";

	return sb.str();
}

static void doBasicDemo()
{
	const char *text = "http://httpbin.org/#/"; // User-supplied text
	const QrCode::Ecc errCorLvl = QrCode::Ecc::LOW;		   // Error correction level

	// Make and print the QR Code symbol
	const QrCode qr = QrCode::encodeText(text, errCorLvl);
	//printQr(qr);

	std::fstream outfile("QRCode.svg", std::ios_base::out);
	if (!outfile)
	{
		std::cout << "--------------------------------------" << std::endl;
		exit(EXIT_FAILURE);
	}
	// std::cout << toSvgString(qr, 4) << std::endl;
	outfile << toSvgString(qr, 4);
}
bool svg2png(const std::string &filename, std::uint32_t width, std::uint32_t height, std::uint32_t backgroundColor)
{
	auto document = Document::loadFromFile(filename);
	if (!document)
	{
		std::cout << filename+"文件加载错误!请确认文件是否存在!" << std::endl;
		exit(EXIT_FAILURE);
	}

	lunasvg::Bitmap bitmap = document->renderToBitmap(width, height, backgroundColor);
	std::string basename = filename.substr(filename.find_last_of("/\\") + 1);

	std::string::iterator ite = find(basename.begin(), basename.end(), '.');
	std::string imgName(basename.begin(), ite);

	imgName.append(".png");
	stbi_write_png(imgName.c_str(), int(bitmap.width()), int(bitmap.height()), 4, bitmap.data(), 0);

	std::cout << "svg to png finished" << std::endl;
}

bool decode(cv::Mat &img)
{
	try
	{
		cv::Mat grayColor;
		if (img.type() != CV_8UC1)
		{
			cv::cvtColor(img, grayColor, COLOR_BGR2GRAY);
		}

		zxing::Ref<zxing::LuminanceSource> source = MatSource::create(grayColor);
		zxing::Ref<zxing::Reader> reader;
		reader.reset(new zxing::qrcode::QRCodeReader);

		zxing::Ref<zxing::Binarizer> binarizer(new zxing::GlobalHistogramBinarizer(source));
		zxing::Ref<zxing::BinaryBitmap> bitmap(new zxing::BinaryBitmap(binarizer));

		zxing::Ref<zxing::Result> result(reader->decode(bitmap, zxing::DecodeHints(zxing::DecodeHints::QR_CODE_HINT)));

		url = result->getText()->getText();

		std::cout << "二维码中包含的内容为:" + url << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
		return false;
	}

	return true;
}
bool GetHttps(std::string &url)
{
	auto resp = requests::get(url.c_str());
	if (resp == NULL)
	{
		std::cout << "requset failed!\n";
		return false;
	}
	else
	{
		std::cout << resp->status_code << " " << resp->status_message() << std::endl;
		std::cout << resp->body.c_str() << std::endl;
		std::fstream urlhtml("urlhtml.html",std::ios_base::out);
		if(!urlhtml)
		{
			std::cout << "urlhtml.html 文件加载错误!请确认文件是否存在!" << std::endl;
			exit(EXIT_FAILURE);
		}
		else{
			urlhtml<<resp->body.c_str()<<std::endl;
		}
		return true;
	}
}
int main()
{
	try
	{
		doBasicDemo();

		std::uint32_t width = 320, height = 320; //svg->png的文件大小;
		std::uint32_t bgColor = 0x00000000;		 //背景颜色;
		std::string filename = "QRCode.svg";
		svg2png(filename, width, height, bgColor);

		cv::Mat img = imread("QRCode.png");
		assert(!img.empty());
		imshow("", img);
		waitKey(2000);
		decode(img);
		if(!url.empty())
			GetHttps(url);
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
	}

	return 0;
}