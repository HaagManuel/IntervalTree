#pragma once

#include <vector>
#include <cassert>
#include "common.h"

struct TreeNode
{
    float split_point;
    int left_node;
    int right_node;
    int start_intervals;
    int end_intervals;

    inline bool has_left() const { return left_node >= 0; }
    inline bool has_right() const { return right_node >= 0; }
};

struct NodeEntry
{
    float x, y;
    int id1, id2;
};

struct ArrayIntervalTree
{
    ArrayIntervalTree(IntervalList _intervals) : intervals(_intervals)
    {
        int root = create_new_node();
        IntervalIdList id_list = get_id_intervals(intervals);
        build_recursive(root, id_list);
    }

    // returns index of new node
    int create_new_node()
    {
        int idx = nodes.size();
        TreeNode node;
        node.left_node = -1;
        node.right_node = -1;
        nodes.push_back(node);
        return idx;
    }

    void build_recursive(int node_idx, IntervalIdList &id_list)
    {
        TreeNode *node = &nodes[node_idx];

        // divide intervals into left, middle and right
        float split_point = get_split_point_median(id_list);
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

        // create middle node
        int n = mid.size();
        node->split_point = split_point;
        node->start_intervals = entries.size();
        node->end_intervals = entries.size() + n;

        std::vector<PointId> x_coordinates, y_coordinates;
        x_coordinates.reserve(n);
        y_coordinates.reserve(n);
        for (auto [intv, id] : mid)
        {
            x_coordinates.emplace_back(intv.start, id);
            y_coordinates.emplace_back(intv.end, id);
        }
        std::sort(x_coordinates.begin(), x_coordinates.end());
        std::sort(y_coordinates.begin(), y_coordinates.end());

        for (int i = 0; i < n; i++)
        {
            entries.emplace_back(x_coordinates[i].point, y_coordinates[i].point, x_coordinates[i].id, y_coordinates[i].id);
        }

        // recurse on left and right part
        if (left.size() > 0)
        {
            // node array might have been reallocated
            int idx = create_new_node();
            nodes[node_idx].left_node = idx;
            build_recursive(idx, left);
        }
        if (right.size() > 0)
        {
            // node array might have been reallocated
            int idx = create_new_node();
            nodes[node_idx].right_node = idx;
            build_recursive(idx, right);
        }
    }

    void stabbed_intervals(float point, std::vector<int> &ids)
    {
        recursive_search(0, point, ids);
    }

    void recursive_search(int node_idx, float point, std::vector<int> &ids)
    {
        TreeNode *node = &nodes[node_idx];
        int start = node->start_intervals;
        int end = node->end_intervals;
        if (point < node->split_point)
        {
            for (int i = start; i < end && entries[i].x <= point; i++)
            {
                ids.push_back(entries[i].id1);
            }
            if (node->has_left())
            {
                recursive_search(node->left_node, point, ids);
            }
        }
        else if (point > node->split_point)
        {
            for (int i = end - 1; i >= start && entries[i].y >= point; i--)
            {
                ids.push_back(entries[i].id2);
            }
            if (node->has_right())
            {
                recursive_search(node->right_node, point, ids);
            }
        }
        else
        {
            for (int i = start; i < end; i++)
            {
                ids.push_back(entries[i].id1);
            }
        }
    }

    int get_depth() const
    {
        return get_depth_rec(0);
    }

    int get_depth_rec(int node_idx) const
    {
        int l = 0;
        int r = 0;
        if (nodes[node_idx].has_left())
            l = get_depth_rec(nodes[node_idx].left_node);
        if (nodes[node_idx].has_right())
            r = get_depth_rec(nodes[node_idx].right_node);

        return 1 + std::max(l, r);
    }

    std::vector<TreeNode> nodes;
    std::vector<NodeEntry> entries;
    IntervalList &intervals;
};