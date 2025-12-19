#ifndef NODE_CPP
#define NODE_CPP

#include "../include/file.hpp"
#include "../include/parser.hpp"
using namespace cheesy;

node::node(node* parent_ptr, bool dir) {
    mother = parent_ptr;
    if (!mother) {
        fs_panic(
            "error: `node` cannot be initialized without a parent directory");
        return;
    }
#ifdef DEBUG
    if (parent_ptr == this) {
        cout << "[warning] node with pointer to itself created" << endl;
    }
#endif

    if (parent_ptr->add_child(this) != exit_code::success) {
        fs_panic(
            "error: 'node' constructor: `parent_ptr` must point to a folder");
        return;
    }

    attributes.directory = dir;

#ifdef DEBUG
    cout << "Node initialized" << endl;

#endif
}

exit_code node::add_child(node* son) {
    if (attributes.directory) {
        son->next_node = data.eldest_child;
        data.eldest_child = son;
        return exit_code::success;
    }

    return exit_code::not_a_directory;
}

exit_code mark_free(const range);

exit_code node::cleanup_file_space() {
    if (attributes.directory) return exit_code::not_a_file;

    for (range& r : data.descriptors) {
        mark_free(r);
        r.len = 0;
    }
    return exit_code::success;
}

exit_code cheesy::create_file(const char* path) {
    // 1. Find the directory.
}

exit_code node::remove_child(int id) {
    if (attributes.directory) {
        node *prev = nullptr, *curr = data.eldest_child;
        while (curr != nullptr && curr->identificator != id) {
            prev = curr;
            curr = curr->next_node;
        }
        if (!curr) return exit_code::no_such_file;

        data.eldest_child = prev;
        if (prev) prev->next_node = curr->next_node;

        return exit_code::success;
    }

    return exit_code::not_a_directory;
}

// node* superblock::get_node_by_path(const char*) {}

exit_code make_readonly(const char* path) {
    node* target = main.get_node_by_path(path);
    target->set_readonly(1);

    return exit_code::success;
}

exit_code cheesy::set_name(const char* path, const char* new_name) {
    node* target = string_utils::find_file(path);
    strcpy(new_name, &(target->name));
    return exit_code::success;
}

#endif
