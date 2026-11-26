typedef struct Content {
    char* path;
    char* content; 
    char* mime_type;
} Content;

int read_file();