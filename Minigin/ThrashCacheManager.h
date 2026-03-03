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
        Transform3D transform;
        int ID;
    };

    class GameObject3DAlt
    {
    public:
        Transform3D* transform;
        int ID;

    };

	class ThrashCacheManager final 
	{
	public:
		static std::vector<float> ThrashCache(int samples) 
		{
            std::vector<float> timesList{};
            GameObject3D* arr = new GameObject3D[1000000]{};
            

            for (int stepsize = 1; stepsize <= samples; stepsize *= 2)
            {
                const auto start = std::chrono::high_resolution_clock::now();
                for (int i = 0; i < 1000000; i += stepsize)
                {
                    arr[i].ID *= 2;
                }
                const auto end = std::chrono::high_resolution_clock::now();
                const auto total = static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());

                std::cout << stepsize << " : " << total << std::endl;
                timesList.emplace_back(total);
            }

            delete[] arr;
            return timesList;
		}

        static std::vector<float> ThrashCacheAlt(int samples)
        {
            std::vector<float> timesList{};
            GameObject3DAlt* arr = new GameObject3DAlt[1000000]{};


            for (int stepsize = 1; stepsize <= samples; stepsize *= 2)
            {
                const auto start = std::chrono::high_resolution_clock::now();
                for (int i = 0; i < 1000000; i += stepsize)
                {
                    arr[i].ID *= 2;
                }
                const auto end = std::chrono::high_resolution_clock::now();
                const auto total = static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());

                std::cout << stepsize << " : " << total << std::endl;
                timesList.emplace_back(total);
            }

            delete[] arr;
            return timesList;
        }
	};




}


