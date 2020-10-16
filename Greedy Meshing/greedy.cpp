
#include <iostream>
#include <array>
#include <vector>

const int SIZE = 2;
const int AREA = SIZE * SIZE;
const int VOL = SIZE * SIZE * SIZE;

enum class Block : uint32_t
{
    Air,
    Dirt,
    Total
};

struct Vertex
{
	int tilingX, tilingY;
    int x, y, z;
};

struct Quad
{
    Quad(const Vertex& v0, const Vertex& v1, const Vertex& v2, const Vertex& v3)
    {
        vertecies[0] = v0;
        vertecies[1] = v1;
        vertecies[2] = v2;
        vertecies[3] = v3;
    }

    Vertex vertecies[4];
};

class GreedyMesher {
public:
    const std::array<bool, VOL>* volume;
    std::vector<Quad> quads;
		// Mask should be of type Block ??
    bool mask[AREA];

    std::vector<Quad> Mesh(const std::array<bool, VOL>& _volume)
    {
		volume = &_volume;
		memset(mask, 0, sizeof(bool) * AREA);
		quads.clear();

        for (int dim = 0; dim < 3; dim++)
        {
            int tangent = (dim + 1) % 3,
                biTangent = (dim + 2) % 3;

            int cursor[3]{ 0 };
            int normal[3]{ 0 };

            normal[dim] = 1;
            for (cursor[dim] = -1; cursor[dim] < SIZE;)
            {
                //Compute mask
                for (cursor[biTangent] = 0; cursor[biTangent] < SIZE; cursor[biTangent]++)
                    for (cursor[tangent] = 0; cursor[tangent] < SIZE; cursor[tangent]++)
                        mask[cursor[biTangent] + cursor[tangent] * SIZE] = calculateMask(dim, cursor, normal);

                cursor[dim]++;
                generateMesh(tangent, biTangent, cursor);
            }
        }

        return quads;
    }

private:
// Return Block
    bool getAtIdx(int i, int j, int k)
    {
        return (*volume)[i + SIZE * (j + SIZE * k)];
    }

// Take in Block, while its the same Block Type
    int computeWidth(int i, int n)
    {
        int w = 1;
        for (; mask[n + w] && i + w < SIZE; w++);
        return w;
    }

// Take in Block, while its the same Block Type
    int computeHeight(int j, int n, int w)
    {
        int h = 1;

        for (; j + h < SIZE; h++)
            for (int k = 0; k < w; k++)
                if (!mask[n + k + h * SIZE])
                    return h;

        return SIZE - j;
    }

    void generateMesh(int tangent, int biTangent, int* cursor)
    {
        int n = 0;

        for (int j = 0; j < SIZE; j++)
            for (int i = 0; i < SIZE;)
            {
                if (mask[n])
                {
				// While its same as mask
                    int w = computeWidth(i, n);
                    int h = computeHeight(j, n, w);

                    //Add quad
                    int du[3]{ 0 };
                    int dv[3]{ 0 };
                    cursor[tangent] = i;
                    cursor[biTangent] = j;
                    du[tangent] = w;
                    dv[biTangent] = h;
                    quads.emplace_back(Quad(
                        Vertex{ w, h, cursor[0], cursor[1], cursor[2] },
                        Vertex{ w, h, cursor[0] + du[0], cursor[1] + du[1], cursor[2] + du[2] },
                        Vertex{ w, h, cursor[0] + du[0] + dv[0], cursor[1] + du[1] + dv[1], cursor[2] + du[2] + dv[2] },
                        Vertex{ w, h, cursor[0] + dv[0], cursor[1] + dv[1], cursor[2] + dv[2] }));
                    //Zero-out mask
                    for (int l = 0; l < h; l++)
                        for (int k = 0; k < w; k++)
                            mask[n + k + l * SIZE] = false;

                    //Increment counters and continue
                    i += w;
                    n += w;
                }
                else
                {
                    i++;
                    n++;
                }
            }
    }

// IDK How or something similar, should mask be of type Block?
    bool calculateMask(int dim, int* cursor, int* normal)
    {
	// Block val1 = (0 <= x[dim]) ? getAtIdx(volume, x[0], x[1], x[2]) : Air;
        bool val1 = (0 <= cursor[dim]) ? getAtIdx(cursor[0], cursor[1], cursor[2]) : false;

	// Block val2 = (x[dim] < SIZE - 1) ? getAtIdx(volume, x[0] + q[0], x[1] + q[1], x[2] + q[2]) : Air;
        bool val2 = (cursor[dim] < SIZE - 1) ? getAtIdx(cursor[0] + normal[0], cursor[1] + normal[1], cursor[2] + normal[2]) : false;

        return val1 != val2;
    }
};

void main()
{
    std::array<bool, VOL> volume{
        true, true, true, true, true, true, true, true };

    auto out = GreedyMesher().Mesh(volume);

    std::cout << out.size() << std::endl;

    std::cin.get();
}
