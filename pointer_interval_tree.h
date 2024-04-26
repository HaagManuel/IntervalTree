#pragma once

#include <vector>
#include <cassert>
#include "common.h"

#define MAX_LEAF_SIZE 100

struct PointerNode
{
    float split_point;
    bool is_splitted;
    PointerNode *left;
    PointerNode *right;
    std::vector<PointId> x_coordinates;
    std::vector<PointId> y_coordinates;

    void create_coordinate_vectors(IntervalIdList &id_list)
    {
        assert(x_coordinates.size() == 0);
        assert(y_coordinates.size() == 0);

        x_coordinates.reserve(id_list.size());
        y_coordinates.reserve(id_list.size());
        for (auto [intv, id] : id_list)
        {
            x_coordinates.push_back({intv.start, id});
            y_coordinates.push_back({intv.end, id});
        }
    }

    void sort_by_point()
    {
        sort(x_coordinates.begin(), x_coordinates.end());
        sort(y_coordinates.begin(), y_coordinates.end());
    }

    void sort_by_id()
    {
        auto cmp = [&](auto &a, auto &b)
        { return a.id < b.id; };
        sort(x_coordinates.begin(), x_coordinates.end(), cmp);
        sort(y_coordinates.begin(), y_coordinates.end(), cmp);
    }

    inline bool has_left() const { return left != nullptr; }
    inline bool has_right() const { return right != nullptr; }
};

struct PointerIntervalTree
{
    PointerIntervalTree(IntervalList &_intervals) : intervals(_intervals)
    {
        root = new PointerNode();

        // give intervals id that is equal to there index
        IntervalIdList id_list = get_id_intervals(intervals);
        build_recursive(root, id_list);
    }

    ~PointerIntervalTree()
    {
        delete_recursive(root);
    }

    void delete_recursive(PointerNode *node)
    {

        if (node->has_left())
        {
            delete_recursive(node->left);
        }
        if (node->has_right())
        {
            delete_recursive(node->right);
        }
        delete node;
    }

    float get_split_point(IntervalIdList &id_list)
    {
        // point and is end
        std::vector<std::pair<float, bool>> points;
        int n = id_list.size();
        points.reserve(2 * n);

        // sort set of start and end points
        for (int i = 0; i < n; i++)
        {
            points.emplace_back(id_list[i].interval.start, false);
            points.emplace_back(id_list[i].interval.end, true);
        }
        std::sort(points.begin(), points.end());

        // sweep line
        int active = 0;
        int left = 0;
        int best_balance = n;
        float best_point = 0;
        for (int i = 0; i < 2 * n - 1; i++)
        {
            auto [p, is_end] = points[i];
            if (!is_end)
            {
                active++;
            }
            else
            {
                active--;
                left++;
            }
            int right = n - active - left;
            int balance = std::abs(left - right);
            if (balance < best_balance)
            {
                best_balance = balance;
                best_point = (p + points[i + 1].first) / 2;
            }
            if (balance == 0 || left > right)
            {
                break;
            }
        }
        return best_point;
    }

    void build_recursive(PointerNode *node, IntervalIdList &id_list)
    {
        if (id_list.size() < MAX_LEAF_SIZE)
        {
            node->is_splitted = false;
            node->create_coordinate_vectors(id_list);
            node->sort_by_id();
            return;
        }

        float split_point = get_split_point(id_list);
        IntervalIdList left, mid, right;
        for (auto [intv, id] : id_list)
        {
            if (intv.end < split_point)
            {
                left.emplace_back(intv, id);
            }
            else if (split_point < intv.start)
            {
                right.emplace_back(intv, id);
            }
            else
            {
                mid.emplace_back(intv, id);
            }
        }

        // build middle node
        node->is_splitted = true;
        node->split_point = split_point;
        node->create_coordinate_vectors(mid);
        node->sort_by_point();

        // recurse on left and right part
        if (left.size() > 0)
        {
            node->left = new PointerNode();
            build_recursive(node->left, left);
        }
        if (right.size() > 0)
        {
            node->right = new PointerNode();
            build_recursive(node->right, right);
        }
    }

    void stabbed_intervals(float point, std::vector<int> &ids)
    {
        recursive_search(root, point, ids);
    }

    void recursive_search(PointerNode *node, float point, std::vector<int> &ids)
    {
        int n = node->x_coordinates.size();

        if (!node->is_splitted)
        {
            for (int i = 0; i < n; i++)
            {
                if (node->x_coordinates[i].point <= point && point <= node->y_coordinates[i].point)
                {
                    ids.push_back(node->x_coordinates[i].id);
                }
            }
            return;
        }

        if (point < node->split_point)
        {
            for (int i = 0; i < n && node->x_coordinates[i].point <= point; i++)
            {
                ids.push_back(node->x_coordinates[i].id);
            }
            if (node->has_left())
            {
                recursive_search(node->left, point, ids);
            }
        }
        else if (point > node->split_point)
        {
            for (int i = n - 1; i >= 0 && node->y_coordinates[i].point >= point; i--)
            {
                ids.push_back(node->y_coordinates[i].id);
            }
            if (node->has_right())
            {
                recursive_search(node->right, point, ids);
            }
        }
        else
        {
            for (int i = 0; i < n; i++)
            {
                ids.push_back(node->x_coordinates[i].id);
            }
        }
    }

    void nodes_per_level()
    {
        std::vector<int> cnt(20, 0);
        std::vector<int> nodes(20, 0);
        rec_node_per_level(root, cnt, nodes, 0);
        for (auto c : cnt)
        {
            if (c > 0)
                std::cout << c << " ";
        }
        std::cout << "\n";
        for (auto c : nodes)
        {
            if (c > 0)
                std::cout << c << " ";
        }
        std::cout << "\n";
        for (int i = 0; i < (int)cnt.size(); i++)
        {
            if (nodes[i] > 0)
            {
                float f = (float)cnt[i] / nodes[i];
                std::cout << f << " ";
            }
        }
        std::cout << "\n";
    }

    void rec_node_per_level(PointerNode *node, std::vector<int> &cnt, std::vector<int> &nodes, int depth)
    {
        cnt[depth] += node->x_coordinates.size();
        nodes[depth]++;
        if (node->has_left())
        {
            rec_node_per_level(node->left, cnt, nodes, depth + 1);
        }
        if (node->has_right())
        {
            rec_node_per_level(node->right, cnt, nodes, depth + 1);
        }
    }

    PointerNode *root;
    IntervalList &intervals;
};