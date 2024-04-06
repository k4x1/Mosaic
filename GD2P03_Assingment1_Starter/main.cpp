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
#include <thread>
#include "Button.h"
#include "ThreadPool.h"
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
    //button
    ThreadPool pool(std::thread::hardware_concurrency());
    int currentPage = 0;
    sf::Font font;
    if (!font.loadFromFile("Avilock.ttf")) {
        // Handle error
        std::cout << "font failed";
    }

    Button pageRight(sf::Vector2f(250, 100), sf::Vector2f(100, 50), font, "Page right", sf::Color::White, sf::Color::Green, sf::Color::Red,PAGE_RIGHT);
    Button pageLeft(sf::Vector2f(100, 100), sf::Vector2f(100, 50), font, "Page left", sf::Color::White, sf::Color::Green, sf::Color::Red, PAGE_LEFT);    
    Button zoomIn(sf::Vector2f(250, 200), sf::Vector2f(100, 50), font, "Zoom in", sf::Color::White, sf::Color::Green, sf::Color::Red,ZOOM_IN);
    Button zoomOut(sf::Vector2f(100, 200), sf::Vector2f(100, 50), font, "Zoom out", sf::Color::White, sf::Color::Green, sf::Color::Red, ZOOM_OUT);
        //



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
    if (!downloader.Download("https://raw.githubusercontent.com/MDS-HugoA/TechLev/main/ImgListLarge.txt", data)) {
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
    int percentageDone = 0;
    grid.InitGrid(imageCount);
    imageTextures.resize(imageCount);
    /*
    auto loadImages = [&](int i) {
        if (!imageTextures[i].loadFromFile(filePaths[i])) {
            std::cout << "Failed to load image: " << filePaths[i] << std::endl;
        }
        else {
            grid.setTileTextures(&imageTextures[i]);
            percentageDone++;
            std::cout << "image set: " << percentageDone << "/" << imageCount << std::endl;
        }
    };
    for (int i = 0; i < imageCount; i++) {
        imageTextures.emplace_back();
        std::cout << "texture num: " << i << std::endl;
    }

    std::vector<std::future<void>> loadFutures;
    for (int i = 0; i < imageCount; i++) {
        std::cout << "img: " << filePaths[i] << std::endl;
        //this for loop probably takes too long, split it into multiple parts
        loadFutures.push_back(std::async(std::launch::async, loadImages, i));
    }
    */
    std::vector<std::future<void>> loadFutures;
    for (int i = 0; i < imageCount; i++) {
        loadFutures.push_back(pool.enqueue([&, i]() {
            if (!imageTextures[i].loadFromFile(filePaths[i])) {
                std::cout << "Failed to load image: " << filePaths[i] << std::endl;
            }
            else {
                grid.setTileTextures(&imageTextures[i]);
                percentageDone++;
                std::cout << "image set: " << percentageDone << "/" << imageCount << std::endl;
            }
            }));
    }

    
    int txtCount = 0;
    auto endTime = std::chrono::steady_clock::now(); // End the timer
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    std::cout << "Total time taken for program to load:  " << elapsedTime << " milliseconds" << std::endl;

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
        for (auto& row : grid.m_grid[currentPage]) {
            for (auto& tile : row) {
      
                window.draw(tile.m_image);
            }
        }
        window.display();

        pageRight.update(buttonWindow.mapPixelToCoords(sf::Mouse::getPosition(buttonWindow)), currentPage, grid);
         pageLeft.update(buttonWindow.mapPixelToCoords(sf::Mouse::getPosition(buttonWindow)), currentPage, grid); 
           zoomIn.update(buttonWindow.mapPixelToCoords(sf::Mouse::getPosition(buttonWindow)), currentPage, grid);
          zoomOut.update(buttonWindow.mapPixelToCoords(sf::Mouse::getPosition(buttonWindow)), currentPage, grid);
        buttonWindow.clear(sf::Color::Blue);
        pageRight.draw(buttonWindow);
        pageLeft.draw(buttonWindow);
        zoomIn.draw(buttonWindow);
        zoomOut.draw(buttonWindow);
        buttonWindow.display();
    }

    curl_global_cleanup();
    return 0;
}
