#include "game.h"
#include "raylib.h"
#include <dlfcn.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

typedef enum { NO_UPDATE, UPDATING, UPDATED } UPDATE_STATE;

#define COPY_BUF_SIZE 4096
#define RELOAD_LIMIT 10

struct stat file_info;
int reload_counter = 0;

UPDATE_STATE get_update_state(const char *file_path);
bool copy_file(const char *src_path, const char *dest_path);
void clean_dll(const char *dll_path);

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("ERROR: Specify the library\n");
        return 1;
    }

    const char *src_dll = argv[argc - 1];
    char dll_path[100];
    char old_dll_path[100];
    void *state = NULL;

    printf("INFO: The dynamic libarary: %s\n", src_dll);

    sprintf(dll_path, "%s_tmp_%d", src_dll, reload_counter++);
    if (!copy_file(src_dll, dll_path)) {
        fprintf(stderr, "ERROR: failed to copy file\n");
        return 1;
    }

    void *handle = dlopen(dll_path, RTLD_LAZY);
    if (handle == NULL) {
        fprintf(stderr, "ERROR: opening library: %s, error: %s\n", dll_path,
                dlerror());
        return 1;
    }

    init_game_t init_game_r = (init_game_t)dlsym(handle, "init_game");
    update_game_t update_game_r = (update_game_t)dlsym(handle, "update_game");
    clear_game_t clear_game_r = (clear_game_t)dlsym(handle, "clear_game");

    state = init_game_r();
    InitWindow(800, 600, "Window");

    while (!WindowShouldClose()) {
        if (get_update_state(src_dll) != UPDATED) {
            BeginDrawing();
            update_game_r(state);
            EndDrawing();
            continue;
        }

        printf("INFO: DLL is updated\n");

        strcpy(old_dll_path, dll_path);
        sprintf(dll_path, "%s_tmp_%d", src_dll, reload_counter++);

        if (!copy_file(src_dll, dll_path)) {
            fprintf(stderr, "ERROR: failed to copy file\n");
            break;
        }

        void *updated_handle = dlopen(dll_path, RTLD_LAZY);
        if (updated_handle == NULL) {
            fprintf(stderr, "ERROR: Handle is null\n");
            fprintf(stderr, "%s\n", dlerror());
            continue;
        }
        CloseWindow();

        init_game_r = (init_game_t)dlsym(updated_handle, "init_game");
        update_game_r = (update_game_t)dlsym(updated_handle, "update_game");
        clear_game_r = (clear_game_t)dlsym(updated_handle, "clear_game");

        InitWindow(800, 600, "Window");
        printf("INFO: New dll found and loaded\n");

        if (dlclose(handle) != 0) {
            fprintf(stderr, "ERROR: Closing old dll\n");
            printf("INFO: Cloding window\n");
            CloseWindow();
            return 1;
        }
        handle = updated_handle;

        clean_dll(old_dll_path);
    }

    CloseWindow();
    clear_game_r(state);

    printf("INFO: Value of dlclose: %d\n", dlclose(handle));
    clean_dll(dll_path);
    return 0;
}

UPDATE_STATE get_update_state(const char *file_path) {
    static UPDATE_STATE update_state = NO_UPDATE;
    static time_t last_updated_time = 0;
    static long int last_file_size = 0;

    if (stat(file_path, &file_info) != 0) {
        fprintf(stderr, "ERROR: Error getting stats\n");
        perror("stat");
        return update_state = NO_UPDATE;
    }

    bool is_time_same = last_updated_time == file_info.st_mtime;
    bool is_size_same = last_file_size == file_info.st_size;

    if (is_time_same && is_size_same) {
        update_state = NO_UPDATE;
    } else if (!is_time_same && (!is_size_same || file_info.st_size == 0)) {
        update_state = UPDATING;

        last_file_size = file_info.st_size;
    } else if (!is_time_same && is_size_same) {
        update_state = UPDATED;
        last_updated_time = file_info.st_mtime;
    }

    return update_state;
}

bool copy_file(const char *src_path, const char *dest_path) {

    printf("INFO: Copying %s into %s\n", src_path, dest_path);

    char buffer[COPY_BUF_SIZE];
    FILE *src, *dest;

    src = fopen(src_path, "rb");
    if (src == NULL) {
        perror("ERROR: opening the src file");
        return false;
    }

    dest = fopen(dest_path, "wb");
    if (dest == NULL) {
        perror("ERROR: opening the dest file");
        return false;
    }
    size_t bytes;

    while ((bytes = fread(buffer, 1, sizeof(buffer), src)) > 0) {
        fwrite(buffer, 1, bytes, dest);
    }

    if (fclose(src) < 0) {
        perror("ERROR: source file cannot be closed");
    }
    if (fclose(dest) < 0) {
        perror("ERROR: destinition file cannot be closed");
    }

    return true;
}

void clean_dll(const char *dll_path) {
    if (remove(dll_path) == 0) {
        printf("INFO: Deleted %s\n", dll_path);
    } else {
        fprintf(stderr, "ERROR DELETING");
        perror(dll_path);
    }
}
