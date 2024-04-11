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
#include "Button.h"
#include "ThreadPool.h"
using StringVector = std::vector<std::string>;
// Function to split URLs from a string
StringVector splitUrls(const std::string& _data) {
    StringVector urls;
    size_t pos = 0;
    size_t oldPos = 0;
    // Loop through the string to find and split URLs
    while ((pos = _data.find('\n', oldPos)) != std::string::npos) {
        urls.push_back(_data.substr(oldPos, pos - oldPos));
        oldPos = pos + 1;
    }
    return urls;
}

int main() {
    // Load font
    sf::Font font;
    if (!font.loadFromFile("Avilock.ttf")) {
        // Handle error
        std::cout << "font failed";
    }

    // Initialize grid
    Grid grid;

    // Initialize buttons
    Button pageRight(sf::Vector2f(250, 100), sf::Vector2f(100, 50), font, "Page right", sf::Color::White, sf::Color::Green, sf::Color::Red, PAGE_RIGHT);
    Button pageLeft(sf::Vector2f(100, 100), sf::Vector2f(100, 50), font, "Page left", sf::Color::White, sf::Color::Green, sf::Color::Red, PAGE_LEFT);
    Button screenshotButton(sf::Vector2f(150, 200), sf::Vector2f(150, 50), font, "     Screenshot", sf::Color::White, sf::Color::Green, sf::Color::Red, SCREENSHOT);

    // Initialize variables for timing and image handling
    std::chrono::steady_clock::time_point startTime;
    std::vector<sf::Texture> imageTextures;
    StringVector filePaths;
    constexpr int windowSize = 900;
    std::atomic<int> currentPage(0);
    bool fileSizeIsSmall = true;

    // Create settings selection window
    {
        sf::RenderWindow settingsSelectWindow(sf::VideoMode(windowSize, windowSize), "select settings");
        // Initialize buttons for settings selection
        Button zoomIn(sf::Vector2f(600, 200), sf::Vector2f(100, 50), font, "Zoom in", sf::Color::White, sf::Color::Green, sf::Color::Red, ZOOM_IN);
        Button zoomOut(sf::Vector2f(200, 200), sf::Vector2f(100, 50), font, "Zoom out", sf::Color::White, sf::Color::Green, sf::Color::Red, ZOOM_OUT);
        Button done(sf::Vector2f(350, 400), sf::Vector2f(200, 50), font, "Done", sf::Color::White, sf::Color::Green, sf::Color::Red, DONE);
        Button fileSize(sf::Vector2f(350, 600), sf::Vector2f(200, 50), font, "file: ", sf::Color::White, sf::Color::Green, sf::Color::Red, FILE_SIZE);
        sf::Text sizeDisplay = sf::Text("Grid Dimensions: ", font, 24);
        sizeDisplay.setPosition(360, 200);

        // Event loop for settings selection window
        while (settingsSelectWindow.isOpen()) {
            sf::Event winEvent;
            while (settingsSelectWindow.pollEvent(winEvent)) {
                if (winEvent.type == sf::Event::Closed || done.update(settingsSelectWindow.mapPixelToCoords(sf::Mouse::getPosition(settingsSelectWindow)), currentPage, grid)) {
                    settingsSelectWindow.close();
                }
                sizeDisplay.setString("Grid Dimensions: " + std::to_string(grid.m_gridSize));
                settingsSelectWindow.clear();
                zoomIn.update(settingsSelectWindow.mapPixelToCoords(sf::Mouse::getPosition(settingsSelectWindow)), currentPage, grid);
                zoomOut.update(settingsSelectWindow.mapPixelToCoords(sf::Mouse::getPosition(settingsSelectWindow)), currentPage, grid);
                fileSize.update(settingsSelectWindow.mapPixelToCoords(sf::Mouse::getPosition(settingsSelectWindow)), currentPage, grid, &fileSizeIsSmall);
                done.draw(settingsSelectWindow);
                settingsSelectWindow.draw(sizeDisplay);
                zoomIn.draw(settingsSelectWindow);
                zoomOut.draw(settingsSelectWindow);
                fileSize.draw(settingsSelectWindow);
                std::string fileText = fileSizeIsSmall ? "img small" : "img large";
                fileSize.m_text.setString("file: " + fileText);
                settingsSelectWindow.display();
            }
        }
    }

    // Initialize main window and button window
    sf::RenderWindow window(sf::VideoMode(windowSize, windowSize), "GD2P03 Assignment 1");
    sf::RenderWindow buttonWindow(sf::VideoMode(windowSize / 2, windowSize / 2), "Buttons");

    // Initialize downloader and thread pool
    std::unique_ptr<CDownloader> downloader = std::make_unique<CDownloader>();
    downloader->Init();
    std::shared_ptr<ThreadPool> pool = std::make_shared<ThreadPool>(std::thread::hardware_concurrency());

    // Determine which file to download based on file size selection
    auto fileToDownload = fileSizeIsSmall ? "https://raw.githubusercontent.com/MDS-HugoA/TechLev/main/ImgListSmall.txt" : "https://raw.githubusercontent.com/MDS-HugoA/TechLev/main/ImgListLarge.txt";
    std::string data;
    if (!downloader->Download(fileToDownload, data)) {
        std::cout << "data failed to download" << std::endl;
        return 1; // Exit if download fails
    }

    // Define lambda function for downloading images
    auto downloadImage = [](const std::string& _url, const std::string& _filePath, CDownloader& _downloader) -> int {
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
        };

    // Split URLs from downloaded data
    StringVector urls = splitUrls(data);
    std::vector<std::future<int>> futures;
    std::mutex countMutex;
    int count = 0;
    startTime = std::chrono::steady_clock::now(); //start timer
    std::vector<std::future<int>> downloadFutures;
    for (const auto& url : urls) {
        std::string filePath = "Images/" + url.substr(url.find_last_of('/') + 1);
        filePaths.push_back(filePath);
        downloadFutures.push_back(pool->enqueue(downloadImage, url, filePath, std::ref(*downloader)));
    }
    for (auto& future : downloadFutures) {
        future.wait();
    }

    // Initialize grid with downloaded images
    int imageCount = urls.size();
    int percentageDone = 0;
    grid.InitGrid(imageCount);
    imageTextures.resize(imageCount);

    // Load images into textures
    std::vector<std::future<void>> loadFutures;
    for (int i = 0; i < imageCount; i++) {
        loadFutures.push_back(pool->enqueue([&, i]() {
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

    // Calculate and display total time taken for program to load
    int txtCount = 0;
    auto endTime = std::chrono::steady_clock::now(); // End the timer
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    std::cout << "Total time taken for program to load: " << elapsedTime << " milliseconds" << std::endl;

    // Main loop for rendering and handling events
    while (window.isOpen() || buttonWindow.isOpen()) {
        
        sf::Event winEvent;
        while (window.pollEvent(winEvent)) {
            if (winEvent.type == sf::Event::Closed) {
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
        grid.AnimateGrid();
        // Update and draw buttons
        pageRight.update(buttonWindow.mapPixelToCoords(sf::Mouse::getPosition(buttonWindow)), currentPage, grid);
         pageLeft.update(buttonWindow.mapPixelToCoords(sf::Mouse::getPosition(buttonWindow)), currentPage, grid); 
         screenshotButton.m_window = &window;
         screenshotButton.update(buttonWindow.mapPixelToCoords(sf::Mouse::getPosition(buttonWindow)), currentPage, grid);
       
        buttonWindow.clear();
        screenshotButton.draw(buttonWindow);
        pageRight.draw(buttonWindow);
        pageLeft.draw(buttonWindow);
       
        buttonWindow.display();
    }

    curl_global_cleanup();
    return 0;
}
