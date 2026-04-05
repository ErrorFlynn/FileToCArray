#include "file_to_c_array.hpp"
#include <sstream>
#include <thread>

std::string file_to_c_array::make_valstr(const char *buf, unsigned width) const
{
	union uint_t
	{
		uint8_t uint8;
		uint16_t uint16;
		uint32_t uint32;
		uint64_t uint64;
	};

	auto pval {reinterpret_cast<const uint_t*>(buf)};
	std::stringstream ss;

	switch(width)
	{
	case 8:
		if(pval->uint64 > 999'999'999'999u)
			ss << std::hex << std::showbase << pval->uint64;
		else ss << pval->uint64;
		break;

	case 4:
		ss << pval->uint32;
		break;

	case 2:
		ss << pval->uint16;
		break;

	case 1:
		ss << std::to_string(pval->uint8);
		break;
	}

	return ss.str();
}


void file_to_c_array::generate(std::filesystem::path in_path, std::filesystem::path out_path)
{
	completed_ = false;
	if(!std::filesystem::exists(in_path))
		throw(std::exception {("Input file does not exist!\n" + in_path.string()).data()});
	std::ifstream infile {in_path, std::ios::binary};
	if(!infile.good())
		throw(std::exception {("Failed to open the input file!\n" + in_path.string() + '\n' + std::strerror(errno)).data()});
	const bool appending {std::filesystem::exists(out_path)};
	std::ofstream outfile {out_path, appending ? std::ios::app : std::ios::out};
	if(!outfile.good())
		throw(std::exception {("Failed to open the output file!\n" + out_path.string() + '\n' + std::strerror(errno)).data()});

	working_ = true;
	auto insize = std::filesystem::file_size(in_path);
	unsigned width = sizeof(uint64_t);
	while(insize % width) width /= 2;
	auto buf {std::make_unique<char[]>(width)};
	const int prog_steps {100};
	uintmax_t prog_step {0}, step_treshold {0};
	if(insize >= prog_steps)
	{
		prog_step = insize / prog_steps;
		step_treshold = prog_step;
	}

	auto name {"arr_" + in_path.filename().string()};
	for(auto &c : name) if(!isalnum(c)) c = '_';

	if(appending)
		outfile << "\n\n";
	else outfile << "#include <cstdint>\n\n";
	if(width > 1) outfile << "// length in bytes: " << insize << "\n";
	outfile << "const uint" << std::to_string(width * 8) << "_t " << name << '[' << std::to_string(insize / width) << "] = {";
	if(line_chars_max) outfile << "\n\t";
	size_t line_chars {0};

	while(infile.read(buf.get(), width))
	{
		auto elstr {make_valstr(buf.get(), width)};
		if(infile.peek() != std::ifstream::traits_type::eof())
			elstr += ',';
		if(line_chars_max)
		{
			line_chars += elstr.size();
			if(line_chars > line_chars_max)
			{
				line_chars = elstr.size();
				outfile << "\n\t";
			}
		}
		outfile << elstr;
		if(cb && prog_step && infile.tellg() >= step_treshold)
		{
			step_treshold += prog_step;
			cb();
		}
		if(abort_) { working_ = false; return; }
		if(outfile.bad())
			throw(std::exception {("Failed trying to write to the output file!\n" + out_path.string() + '\n' + std::strerror(errno)).data()});
	}

	if(infile.bad())
		throw(std::exception {("Failed trying to read the input file!" + in_path.string() + '\n' + std::strerror(errno)).data()});

	if(line_chars_max)
		outfile << '\n';
	outfile << "};";
	working_ = false;
	completed_ = true;
}


void file_to_c_array::abort()
{
	abort_ = true;
	while(working_)
		std::this_thread::sleep_for(std::chrono::milliseconds {50});
	abort_ = false;
}
