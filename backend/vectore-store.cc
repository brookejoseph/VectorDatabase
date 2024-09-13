#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <limits>

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
        Vector *closestVector = NULL;
        float minDistance = std::numeric_limits<float>::max();

        for (size_t i = 0; i < vectors.size(); ++i)
        {
            float dist = euclideanDistance(vectors[i].values, query);
            if (dist < minDistance)
            {
                minDistance = dist;
                closestVector = &vectors[i];
            }
        }
        return closestVector;
    }

    void printVectors()
    {
        for (size_t i = 0; i < vectors.size(); ++i)
        {
            std::cout << "ID: " << vectors[i].id << " Values: ";
            for (size_t j = 0; j < vectors[i].values.size(); ++j)
            {
                std::cout << vectors[i].values[j] << " ";
            }
            std::cout << std::endl;
        }
    }
};

int main()
{
    VectorStore store;

    std::vector<float> vec1;
    vec1.push_back(1.0);
    vec1.push_back(2.0);
    vec1.push_back(3.0);
    store.addVector(Vector(1, vec1));

    std::vector<float> vec2;
    vec2.push_back(4.0);
    vec2.push_back(5.0);
    vec2.push_back(6.0);
    store.addVector(Vector(2, vec2));

    std::vector<float> vec3;
    vec3.push_back(7.0);
    vec3.push_back(8.0);
    vec3.push_back(9.0);
    store.addVector(Vector(3, vec3));

    std::cout << "All vectors:" << std::endl;
    store.printVectors();

    std::vector<float> query;
    query.push_back(2.0);
    query.push_back(3.0);
    query.push_back(4.0);

    Vector *closest = store.findClosestVector(query);
    if (closest)
    {
        std::cout << "Closest vector to query: ID = " << closest->id << std::endl;
    }
    else
    {
        std::cout << "No vectors in the store." << std::endl;
    }

    return 0;
}