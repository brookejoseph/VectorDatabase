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
private:
    std::vector<Vector> vectors;

public:
    // Method to add a vector to the store
    void addVector(const Vector &vec)
    {
        vectors.push_back(vec);
    }

    // Method to retrieve a vector by its ID
    Vector *getVectorById(int id)
    {
        for (auto &vec : vectors)
        {
            if (vec.id == id)
            {
                return &vec;
            }
        }
        return nullptr; // return null if not found
    }

    // Optional: method to print vectors (for debugging purposes)
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
