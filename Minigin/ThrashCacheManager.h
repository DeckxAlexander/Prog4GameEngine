#pragma once
#include "Singleton.h"
#include <iostream>
#include <vector>
#include <chrono>

namespace dae
{
    struct Transform3D
    {
        float matrix[16] = {
            1,0,0,0,
            0,1,0,0,
            0,0,1,0,
            0,0,0,1 };
    };


    class GameObject3D
    {
    public:
        Transform3D transform{};
        int ID{1};
    };

    class GameObject3DAlt
    {
    public:
        Transform3D* transform{ };
        int ID{ 1 };

    };

	class ThrashCacheManager final 
	{
	public:
		static std::vector<float> ThrashCache(int samples) 
		{
            std::vector<float> timesList;
            timesList.resize(11);

            for (int index{  }; index < samples; index++)
            {

                int yIndex{};
                GameObject3D* arr = new GameObject3D[1000000]{};
                for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
                {

                    const auto start = std::chrono::high_resolution_clock::now();
                    for (int i = 0; i < 1000000; i += stepsize)
                    {
                        arr[i].ID *= 2;
                    }
                    const auto end = std::chrono::high_resolution_clock::now();
                    const auto total = static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());


                    timesList[yIndex] += total;
                    yIndex++;

                }
                delete[] arr;
            }

            for (float& times : timesList)
            {
                times /= samples;
                std::cout << times << std::endl;
            }

            return timesList;
		}

        static std::vector<float> ThrashCacheAlt(int samples)
        {
            std::vector<float> timesList;
            timesList.resize(11);

            for (int index{}; index < samples; index++)
            {
               
                int yIndex{};
                GameObject3DAlt* arr = new GameObject3DAlt[1000000]{};
                for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
                {
                    
                    const auto start = std::chrono::high_resolution_clock::now();
                    for (int i = 0; i < 1000000; i += stepsize)
                    {
                        arr[i].ID *= 2;
                    }
                    const auto end = std::chrono::high_resolution_clock::now();
                    const auto total = static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());


                    timesList[yIndex] += total;
                    yIndex++;
                   
                }

                delete[] arr;
            }
            
            for (float& times : timesList) 
            {
                times /= samples;
                std::cout << times << std::endl;
            }

            return timesList;
        }


        static std::vector<float> ThrashCacheInt(int samples)
        {
            std::vector<float> timesList;
            timesList.resize(11);

            for (int index{}; index < samples; index++)
            {

                int yIndex{};
                int* arr = new int[1000000]{};
                for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
                {

                    const auto start = std::chrono::high_resolution_clock::now();
                    for (int i = 0; i < 1000000; i += stepsize)
                    {
                        arr[i] *= 2;
                    }
                    const auto end = std::chrono::high_resolution_clock::now();
                    const auto total = static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());


                    timesList[yIndex] += total;
                    yIndex++;

                }
                delete[] arr;
            }

            for (float& times : timesList)
            {
                times /= samples;
                std::cout << times << std::endl;
            }

            return timesList;
        }
	};




}


