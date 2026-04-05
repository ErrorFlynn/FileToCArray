#pragma once

#include <fstream>
#include <sstream>
#include <functional>
#include <filesystem>

class file_to_c_array
{
	std::function<void(void)> cb;
	std::atomic<bool> working_ {false}, abort_ {false}, completed_ {false};
	unsigned line_chars_max {120};

	std::string make_valstr(const char *buf, unsigned width) const;

public:

	file_to_c_array() = default;

	void abort(); // aborts operation when called from a different thread
	bool working() { return working_; } // false if generate() not running
	bool completed() { return completed_; } // true if last operation completed (was not aborted and did not error out)
	auto max_chars_per_line() const { return line_chars_max; }
	void max_chars_per_line(unsigned chars) { line_chars_max = chars; }
	void generate(std::filesystem::path in_path, std::filesystem::path out_path);

	// gets called for each 1% of progress; argument is copied and stored in data member `cb`
	void progress_callback(const std::function<void(void)> callback) { cb = callback; }
};