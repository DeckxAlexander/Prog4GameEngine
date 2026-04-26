#include "ThrashCacheComponent.h"
#include <imgui.h>
#include <algorithm>
#include <numeric>
#include <imgui_plot.h>


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
    int ID{ 1 };
};

class GameObject3DAlt
{
public:
    Transform3D* transform{ };
    int ID{ 1 };

};






void dae::ThrashCacheComponent::Update()
{
}

void dae::ThrashCacheComponent::Render() const
{


    RenderImGuiGameObject();
    RenderImGuiInt();


}

void dae::ThrashCacheComponent::RenderImGuiGameObject() const
{
    ImGui::Begin("Exercise 2");

    // Samples input
    ImGui::PushItemWidth(120);
    ImGui::InputInt("##samples", &m_Samples);
    ImGui::PopItemWidth();
    ImGui::SameLine();
    ImGui::Text("# samples");
    m_Samples = std::max(1, m_Samples);

    const float* combined_data[2];
    std::vector<float> x_values;
    ImU32 colors[2] = { ImColor(0, 255, 0), ImColor(255, 0, 0) };

    int dataCount = 0;
    float maxY = 0.0f;


    //Config
    float plotWidth = ImGui::GetContentRegionAvail().x;
    ImGui::PlotConfig conf;

    conf.values.colors = colors;
    conf.scale.min = 0.0f;
    conf.tooltip.show = true;
    conf.grid_x.show = true;
    conf.grid_y.show = true;
    conf.line_thickness = 2.0f;
    conf.frame_size = ImVec2(plotWidth, 150);

    //Thrash Cache 
    if (ImGui::Button("Thrash the cache with GameObject3D"))
    {
        m_Timings3D.clear();
        m_Timings3D = ThrashCache(m_Samples);
    }


    if (!m_Timings3D.empty())
    {
        const float* data[1] = { m_Timings3D.data() };
        combined_data[0] = m_Timings3D.data();
        dataCount = static_cast<int>(m_Timings3D.size());
        maxY = *std::max_element(m_Timings3D.begin(), m_Timings3D.end());
        x_values.resize(dataCount);
        for (int i = 0; i < dataCount; ++i)
        {
            x_values[i] = std::pow(2.0f, static_cast<float>(i));
        }
        conf.values.xs = x_values.data();
        conf.values.ys_list = data;
        conf.values.ys_count = 1;
        conf.values.count = dataCount;
        conf.scale.max = maxY;


        ImGui::Plot("Graph 3D", conf);

    }


    //Thrash Cache Alt
    if (ImGui::Button("Thrash the cache with GameObject3DAlt"))
    {
        m_Timings3DAlt.clear();
        m_Timings3DAlt = ThrashCacheAlt(m_Samples);
    }

    if (!m_Timings3DAlt.empty())
    {
        const float* data[1] = { m_Timings3DAlt.data() };
        combined_data[1] = m_Timings3DAlt.data();
        int altSize = static_cast<int>(m_Timings3DAlt.size());
        dataCount = std::max(dataCount, altSize);
        float altMax = *std::max_element(m_Timings3DAlt.begin(), m_Timings3DAlt.end());
        maxY = std::max(maxY, altMax);

        x_values.resize(altSize);
        for (int i = 0; i < altSize; ++i)
        {
            x_values[i] = std::pow(2.0f, static_cast<float>(i));
        }
        conf.values.xs = x_values.data();
        conf.values.ys_list = data;
        conf.values.ys_count = 1;
        conf.values.count = altSize;
        conf.scale.max = altMax;
        ImGui::Plot("Graph Alt", conf);

    }

    if (!m_Timings3D.empty() && !m_Timings3DAlt.empty())
    {
        ImGui::Text("Combined: ");
        x_values.resize(dataCount);
        for (int i = 0; i < dataCount; ++i)
        {
            x_values[i] = std::pow(2.0f, static_cast<float>(i));
        }
        conf.values.xs = x_values.data();
        conf.values.ys_list = combined_data;
        conf.values.ys_count = 2;
        conf.values.count = dataCount;
        conf.scale.max = maxY;

        ImGui::Plot("Graph Combined", conf);

    }






    ImGui::End();
}

void dae::ThrashCacheComponent::RenderImGuiInt() const
{
    ImGui::Begin("Exercise 1");

    // Samples input
    ImGui::PushItemWidth(120);
    ImGui::InputInt("##samplesInt", &m_SamplesInt);
    ImGui::PopItemWidth();
    ImGui::SameLine();
    ImGui::Text("# samples");

    m_SamplesInt = std::max(1, m_SamplesInt);

    //Thrash Cache 
    if (ImGui::Button("Thrash the cache with GameObject3D"))
    {

        m_TimingsInt.clear();
        m_TimingsInt = ThrashCacheInt(m_SamplesInt);


    }
    if (!m_TimingsInt.empty())
    {
        const float* y_data[1] = { m_TimingsInt.data() };
        std::vector<float> x_values;
        ImU32 colors[1] = { ImColor(0, 255, 0) };
        int dataCount = static_cast<int>(m_TimingsInt.size());
        float maxY = *std::max_element(m_TimingsInt.begin(), m_TimingsInt.end());

        //Get X values
        x_values.resize(dataCount);
        for (int i = 0; i < dataCount; ++i)
        {
            x_values[i] = std::pow(2.0f, static_cast<float>(i));
        }

        //Config
        float plotWidth = ImGui::GetContentRegionAvail().x;
        ImGui::PlotConfig conf;
        conf.values.xs = x_values.data();
        conf.values.ys_list = y_data;
        conf.values.ys_count = 1;
        conf.values.count = dataCount;
        conf.values.colors = colors;
        conf.scale.min = 0.0f;
        conf.scale.max = maxY;

        conf.tooltip.show = true;
        conf.grid_x.show = true;
        conf.grid_y.show = true;

        conf.line_thickness = 2.0f;
        conf.frame_size = ImVec2(plotWidth, 150);

        ImGui::Plot("Performance", conf);

    }


    ImGui::End();
}

std::vector<float> dae::ThrashCacheComponent::ThrashCache(int samples)
{
    std::vector<float> averageTimesList{};
    averageTimesList.resize(11);


    GameObject3D* arr = new GameObject3D[1000000]{};
    int yindex{};
    for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
    {
        std::vector<float> timesList;
        for (int index{  }; index < samples; index++)
        {
            const auto start = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < 1000000; i += stepsize)
            {
                arr[i].ID *= 2;
            }
            const auto end = std::chrono::high_resolution_clock::now();
            const auto total = static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
            timesList.push_back(total);
        }


        std::sort(timesList.begin(), timesList.end());

        timesList.erase(timesList.begin());
        timesList.pop_back();
        

        averageTimesList[yindex] = std::accumulate(timesList.begin(), timesList.end(), 0.f) / static_cast<float>(samples);
        std::cout << std::accumulate(timesList.begin(), timesList.end(), 0.f) / samples << std::endl;
        yindex++;
    }
    delete[] arr;
    return averageTimesList;

}

std::vector<float> dae::ThrashCacheComponent::ThrashCacheAlt(int samples) 
{
    std::vector<float> averageTimesList{};
    averageTimesList.resize(11);


    GameObject3DAlt* arr = new GameObject3DAlt[1000000]{};
    int yindex{};
    for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
    {
        std::vector<float> timesList;
        for (int index{  }; index < samples; index++)
        {
            const auto start = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < 1000000; i += stepsize)
            {
                arr[i].ID *= 2;
            }
            const auto end = std::chrono::high_resolution_clock::now();
            const auto total = static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
            timesList.push_back(total);
        }


        std::sort(timesList.begin(), timesList.end());

        timesList.erase(timesList.begin());
        timesList.pop_back();


        averageTimesList[yindex] = std::accumulate(timesList.begin(), timesList.end(), 0.f) / static_cast<float>(samples);
        std::cout << std::accumulate(timesList.begin(), timesList.end(), 0.f) / samples << std::endl;
        yindex++;
    }
    delete[] arr;
    return averageTimesList;
}


std::vector<float> dae::ThrashCacheComponent::ThrashCacheInt(int samples) 
{

    std::vector<float> averageTimesList{};
    averageTimesList.resize(11);


    int* arr = new int[1000000]{};
    int yindex{};
    for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
    {
        std::vector<float> timesList;
        for (int index{  }; index < samples; index++)
        {
            const auto start = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < 1000000; i += stepsize)
            {
                arr[i] *= 2;
            }
            const auto end = std::chrono::high_resolution_clock::now();
            const auto total = static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
            timesList.push_back(total);
        }


        std::sort(timesList.begin(), timesList.end());

        timesList.erase(timesList.begin());
        timesList.pop_back();


        averageTimesList[yindex] = std::accumulate(timesList.begin(), timesList.end(), 0.f) / static_cast<float>(samples);
        std::cout << std::accumulate(timesList.begin(), timesList.end(), 0.f) / samples << std::endl;
        yindex++;
    }
    delete[] arr;
    return averageTimesList;

}

dae::ThrashCacheComponent::ThrashCacheComponent(GameObject* pOwner) : ObjectComponent(pOwner)
{
}


