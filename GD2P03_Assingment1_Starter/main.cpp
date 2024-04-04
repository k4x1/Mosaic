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
#include "Grid.h"

// Function to take a screenshot
void screenshot(const std::string& _fileSaveLocation, sf::Window* _window) {
    sf::Texture texture;
    texture.create(_window->getSize().x, _window->getSize().y);
    texture.update(*_window);
    if (texture.copyToImage().saveToFile(_fileSaveLocation)) {
        std::cout << "screenshot saved to " << _fileSaveLocation << std::endl;
    }
}

// Function to split URLs from a string
std::vector<std::string> splitUrls(const std::string& _data) {
    std::vector<std::string> urls;
    size_t pos = 0;
    size_t oldPos = 0;
    while ((pos = _data.find('\n', oldPos)) != std::string::npos) {
        urls.push_back(_data.substr(oldPos, pos - oldPos));
        oldPos = pos + 1;
    }
    return urls;
}

// Function to download images asynchronously
int downloadImage(const std::string& _url, const std::string& _filePath, CDownloader& _downloader) {
    std::ifstream file(_filePath);
    if (!file.good()) {
        if (_downloader.DownloadToFile(_url.c_str(), _filePath.c_str())) {
            std::cout << "image download success: " << _url << std::endl;

        }
        else {
            std::cout << "image download FAILED: " << _url << std::endl;
        }
    }
    return 0;
}

int main() {
    std::chrono::steady_clock::time_point startTime;

    std::vector<sf::Texture> imageTextures;
    std::vector<std::string> filePaths;
    const int windowSize = 900;
    sf::RenderWindow window(sf::VideoMode(windowSize, windowSize), "GD2P03 Assignment 1");
    sf::RenderWindow buttonWindow(sf::VideoMode(windowSize/2, windowSize/2), "Buttons");
    CDownloader downloader;
    downloader.Init();

    //small
    //https://raw.githubusercontent.com/MDS-HugoA/TechLev/main/ImgListSmall.txt
    //large
    //https://raw.githubusercontent.com/MDS-HugoA/TechLev/main/ImgListLarge.txt
    std::string data;
    if (!downloader.Download("https://raw.githubusercontent.com/MDS-HugoA/TechLev/main/ImgListSmall.txt", data)) {
        std::cout << "data failed to download" << std::endl;
        return 1; // Exit if download fails
    }

    std::vector<std::string> urls = splitUrls(data);
    std::vector<std::future<int>> futures;
    std::mutex countMutex;
    int count = 0;
    startTime = std::chrono::steady_clock::now();
    for (const auto& url : urls) {
        std::string filePath = "Images/" + url.substr(url.find_last_of('/') + 1);
        filePaths.push_back(filePath);
        std::cout << filePath << std::endl;
        futures.push_back(std::async(std::launch::async, downloadImage, url, filePath, std::ref(downloader)));

    }
    
    for (auto& future : futures) {
        future.get(); // Wait for all threads to finish
    }
    //timer things
   

    // Initialize grid
    Grid grid;
    int imageCount = urls.size();
    grid.InitGrid(imageCount);

    for (int i = 0; i < imageCount; i++) {
        imageTextures.emplace_back();
    }
    
    std::vector<std::future<void>> loadFutures;
    for (int i = 0; i < imageCount; i++) {
        std::cout << "img: " << filePaths[i] << std::endl;

        loadFutures.push_back(std::async(std::launch::async, [&, i]() {
            if (!imageTextures[i].loadFromFile(filePaths[i])) {
                std::cout << "Failed to load image: " << filePaths[i] << std::endl;
            }
            else {
                grid.setTileTextures(&imageTextures[i]);
            }
            }));
    }
    /*
    for (auto& future : loadFutures) {
        future.get();
    }*/

    
    int txtCount = 0;
    auto endTime = std::chrono::steady_clock::now(); // End the timer
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    std::cout << "Total time taken for program to load:  " << elapsedTime << " milliseconds" << std::endl;
    /*
    for (int page = 0; page < grid.m_pageCount-1; page++) {
        for (auto& row : grid.m_grid[page]) {
            for (auto& tile : row) {
                std::cout << "texture set: " << txtCount<<std::endl;
                tile.setTexture(&imageTextures[txtCount]);
                txtCount++;
            }
        }
    }*/
    

    // Main loop
    while (window.isOpen() || buttonWindow.isOpen()) {
        sf::Event winEvent;
        while (window.pollEvent(winEvent)) {
            if (winEvent.type == sf::Event::Closed) {
                screenshot("something.png", &window);
                window.close();
            }
        }
        while (buttonWindow.pollEvent(winEvent)) {
            if (winEvent.type == sf::Event::Closed) {
                buttonWindow.close();
            }
        }
        window.clear();
        for (auto& row : grid.m_grid[0]) {
            for (auto& tile : row) {
                window.draw(tile.m_image);
            }
        }
        window.display();

        buttonWindow.clear(sf::Color::Blue);
        buttonWindow.display();
    }

    curl_global_cleanup();
    return 0;
}
