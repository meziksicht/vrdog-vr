// Modifications made by K. Kameník - Meziksicht, 2025.
// Copyright (c) 2017-2025 The Khronos Group Inc.
//
// SPDX-License-Identifier: Apache-2.0
#include <Windows.h>
#include "pch.h"
#include "common.h"
#include "options.h"
#include "platformdata.h"
#include "platformplugin.h"
#include "graphicsplugin.h"
#include "openxr_program.h"
#include "opencv2/opencv.hpp"
#include "FFmpegReader.h"

int main(int argc, char* argv[]) {
    try {
        //FFmpegReader readerFFMPEG("rtsp://127.0.0.1:5000");
        std::shared_ptr<Options> options = std::make_shared<Options>();
        std::shared_ptr<PlatformData> data = std::make_shared<PlatformData>();

        // Spawn a thread to wait for a keypress
        static bool quitKeyPressed = false;
        auto exitPollingThread = std::thread{ [] {
            Log::Write(Log::Level::Info, "Press any key to shutdown...");
            (void)getchar();
            quitKeyPressed = true;
        } };
        exitPollingThread.detach();

        bool requestRestart = false;
        do {
            // Create platform-specific implementation.
            std::shared_ptr<IPlatformPlugin> platformPlugin = CreatePlatformPlugin(options, data);

            // Create graphics API implementation.
            std::shared_ptr<IGraphicsPlugin> graphicsPlugin = CreateGraphicsPlugin(options, platformPlugin);

            // Initialize the OpenXR program.
            std::shared_ptr<IOpenXrProgram> program = CreateOpenXrProgram(options, platformPlugin, graphicsPlugin);

            program->CreateInstance();
            program->InitializeSystem();

            options->SetEnvironmentBlendMode(program->GetPreferredBlendMode());
            platformPlugin->UpdateOptions(options);
            graphicsPlugin->UpdateOptions(options);

            program->InitializeDevice();
            program->InitializeSession();
            program->CreateSwapchains();


            //cv::VideoCapture cap;
            cv::Mat frame;

            //cap.open(0);
            //cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
            //cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);

            //cap >> frame;
            while (!quitKeyPressed) {
                bool exitRenderLoop = false;
                //if (readerFFMPEG.receiveFrame(frame)) {
                    //if (!frame.empty())
                    //{
                    //cv::cvtColor(frame, frame, cv::COLOR_BGR2RGBA);
                    //graphicsPlugin->SaveFrame(frame);
                    //cap >> frame;
                //}
                //}

                program->PollEvents(&exitRenderLoop, &requestRestart);
                if (exitRenderLoop) {
                    break;
                }

                if (program->IsSessionRunning()) {
                    program->PollActions();
                    program->RenderFrame();
                }
                else {
                    // Throttle loop since xrWaitFrame won't be called.
                    std::this_thread::sleep_for(std::chrono::milliseconds(250));
                }
            }
        } while (!quitKeyPressed && requestRestart);

        return 0;
    }
    catch (const std::exception& ex) {
        Log::Write(Log::Level::Error, ex.what());
        return 1;
    }
    catch (...) {
        Log::Write(Log::Level::Error, "Unknown Error");
        return 1;
    }
}