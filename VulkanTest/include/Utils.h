#ifndef UTILS_H
#define UTILS_H

#include <vector>

struct GLFWwindow;

class Utils
{
public:
	static std::vector<char> readFile(const std::string& filename);
	static double calcFPS(GLFWwindow* window, double theTimeInterval = 1.0, std::string theWindowTitle = "Vulkan");
};

#endif