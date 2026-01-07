#ifndef NODE_CPP
#define NODE_CPP
#include <file.hxx>
#include <parser.hxx>
#include <superblock.hxx>
namespace quarkie {

exit_code node::init(node* parent_ptr, bool dir_indicator) {
    mother = parent_ptr;
    if (! mother) {
        sb.external_interface->fs_panic(
            exit_code::invalid_parental_dir,
            "error: a unit cannot be initialized without a parent directory");
        return exit_code::invalid_parental_dir;
    }
#ifdef DEBUG
    if (parent_ptr == this) {
        cout << __PRETTY_FUNCTION__
             << ": [warning] node with pointer to itself created" << endl;
    }
#endif

    if (parent_ptr->add_child(this) != exit_code::success) {
        sb.external_interface->fs_panic(
            exit_code::invalid_parental_dir,
            "error: 'node' constructor: `parent_ptr` must point to a folder");
        return exit_code::invalid_parental_dir;
    }

    is_directory = dir_indicator;
#ifdef DEBUG
    cout << "Node initialized" << endl;
#endif
    return exit_code::success;
}

exit_code node::add_child(node* son) {
    if (this->is_directory) {
        son->next_node = data.children.eldest_child;
        data.children.eldest_child = son;
        return exit_code::success;
    }

    return exit_code::not_a_directory;
}

exit_code mark_free(const range);
exit_code mark_free(const sector_no start, const uint length);

exit_code node::cleanup_file_space() {
    if (this->is_directory)
        return exit_code::not_a_file;

    else if (this->fragmented) {
        free_space::meta_sector meta;
        sb.external_interface->read_blocks(reinterpret_cast<char*>(&meta),
                                           data.more_descriptors, 1);
    } else {
        for (range& r : data.descriptors) {
            mark_free(r);
            r.length = 0;
        }
    }
    return exit_code::success;
}

exit_code node::remove_child(const int id, bool dir) {
    if (this->is_directory) {
        node *prev = nullptr, *curr = dir ? data.children.eldest_child
                                          : data.children.first_subdir;

        while (curr != nullptr) {
            prev = curr;
            curr = curr->next_node;
            if (curr->identificator == id) break;
        }

        this->data.children.eldest_child = prev;
        if (prev) prev->next_node = curr->next_node;
        sb.node_allocator.free_slot(curr);
        return exit_code::success;
    }

    return exit_code::not_a_directory;
}

exit_code node::remove_child_by_ptr(node* ptr) {}

// node* superblock::get_node_by_path(const char*) {}

exit_code make_readonly(const char* path) {
    node* target = sb.get_node_by_path(path);
    target->set_readonly(true);

    return exit_code::success;
}

exit_code quarkie::set_name(const char* path, const char* new_name) {
    node* target = string_utils::find_file(path);
    __builtin_strcpy((char*) &target->name, new_name);

    return exit_code::success;
}

}  // namespace quarkie
#endif
