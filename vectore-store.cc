#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

class Vector
{
public:
    int id;
    std::vector<float> values;
    Vector(int id, std::vector<float> values) : id(id), values(values) {}
};

class VectorStore
{
    float euclideanDistance(const std::vector<float> &vec1, const std::vector<float> &vec2)
    {
        float sum = 0.0;
        for (size_t i = 0; i < vec1.size(); ++i)
        {
            sum += (vec1[i] - vec2[i]) * (vec1[i] - vec2[i]);
        }
        return std::sqrt(sum);
    }

private:
    std::vector<Vector> vectors;

public:
    void addVector(const Vector &vec)
    {
        vectors.push_back(vec);
    }

    Vector *findClosestVector(const std::vector<float> &query)
    {
        Vector *closestVector = nullptr;
        float minDistance = std::numeric_limits<float>::max();

        for (auto &vec : vectors)
        {
            float dist = euclideanDistance(vec.values, query);
            if (dist < minDistance)
            {
                minDistance = dist;
                closestVector = &vec;
            }
        }
        return closestVector;
    }

    Vector *getVectorById(int id)
    {
        for (auto &vec : vectors)
        {
            if (vec.id == id)
            {
                return &vec;
            }
        }
        return nullptr;
    }

    void printVectors()
    {
        for (const auto &vec : vectors)
        {
            std::cout << "ID: " << vec.id << " Values: ";
            for (float val : vec.values)
            {
                std::cout << val << " ";
            }
            std::cout << std::endl;
        }
    }
};
