#define NOMINMAX
#include "Downloader.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <vector>
#include <future>
#include <mutex>
#include <memory>
#include "FileImages.h"

void screenshot(std::string fileSaveLocation, sf::Window* window)
{
	sf::Texture texture;
	texture.create(window->getSize().x, window->getSize().y);
	texture.update(*window);
	if (texture.copyToImage().saveToFile(fileSaveLocation))
	{
		std::cout << "screenshot saved to " << fileSaveLocation << std::endl;
	}
}
int main()
{
	std::chrono::steady_clock::time_point startTime;
	int windowSize = 900;
	int imageCount = 9;
	sf::RenderWindow window(sf::VideoMode(windowSize, windowSize), "GD2P03 Assignment 1");
	
	CDownloader downloader;
	downloader.Init();

	std::string data = "";
	if (downloader.Download(
		"https://raw.githubusercontent.com/MDS-HugoA/TechLev/main/ImgListSmall.txt",
		data))
	{
		std::cout << data << "\n";
		printf("data length: %zd\n", data.length());
	}
	else
	{
		printf("data failed to download");
	}
	
	int imageSize = windowSize / sqrt(imageCount);
	
	//split the urls
	size_t pos = 0;
	size_t oldPos = 0;
	std::vector<std::string> urls;
	while (pos != std::string::npos)
	{
		pos = data.find('\n', oldPos);
		if (oldPos < data.length())
		{
			urls.push_back(data.substr(oldPos, pos - oldPos));
			printf("url [%zd] : %s\n", urls.size() - 1, urls[urls.size() - 1].c_str());
			oldPos = pos + 1;
		}
	} 

	std::vector<sf::RectangleShape> imageArray(imageCount);
	std::vector<sf::Texture> textArray(imageCount);
	textArray.reserve(urls.size());
	std::mutex countMutex;
	startTime = std::chrono::steady_clock::now();
	int count = 0;

	auto DownloadImageToFile = [&]() {

		int localCount;
		{
			std::lock_guard<std::mutex> lock(countMutex);
			localCount = count;
			count++;
		}
		std::string filePath = "Images/" + urls[localCount].substr(urls[localCount].find_last_of('/') + 1);

		std::ifstream file(filePath);
		if (!file.good()) {
			if (downloader.DownloadToFile(urls[localCount].c_str(), filePath.c_str())) {


				std::cout << "image download success: " << localCount << std::endl;

			}
		}
	//	FileImages newImage(filePath);
		//grid.addTile(newImage);
		return 0;
		};

	
	std::vector<std::future<int>> futures;
	for (int i = 0; i < 3; i++) {

		for (int j = 0; j < 3; j++) {
			std::cout << i << " | " << j<< std::endl;
			futures.push_back(std::async(std::launch::async, DownloadImageToFile));
		}
	}
	std::cout << " threads started" << std::endl;
	int countStart = 0;
	for (auto& future : futures) {
		future.get(); // Wait for all threads to finish

		countStart++;
		std::cout<<countStart<<std::endl;
	}
	
	auto endTime = std::chrono::steady_clock::now(); // End the timer
	auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
	std::cout << "Total time taken to load images: " << elapsedTime << " milliseconds" << std::endl;

                 
	while (window.isOpen())
	{
		sf::Event winEvent;
		while (window.pollEvent(winEvent))
		{
			if (winEvent.type == sf::Event::Closed)
			{
				screenshot("something.png", &window);
				window.close();
			}
		}

		window.clear();
		for (int i = 0; i < imageCount; i++) {
			
			window.draw(imageArray[i]);
		}
		
		window.display();
	}

	curl_global_cleanup();

	return 0;
}