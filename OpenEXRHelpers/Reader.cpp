#include "OpenEXRIncludes.h"

RGBABuffer OpenEXRReader::readRGBA(std::string filename){
	RGBABuffer buffer;


	try
	{
		Imf::RgbaInputFile in(filename.c_str());

		Imath::Box2i win = in.dataWindow();

		buffer.dim = Imath::V2i(win.max.x - win.min.x + 1,
			win.max.y - win.min.y + 1);

		buffer.buffer = new Imf::Rgba[buffer.dim.x * buffer.dim.y];

		int dx = win.min.x;
		int dy = win.min.y;

		in.setFrameBuffer(buffer.buffer - dx - dy * buffer.dim.x, 1, buffer.dim.x);
		in.readPixels(win.min.y, win.max.y);
	}
	catch(Iex::BaseExc & e)
	{
		std::cerr << e.what() << std::endl;
		assert(false && e.what());
		return buffer;
	}
	return buffer;
}