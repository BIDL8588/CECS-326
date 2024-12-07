#include <stdio.h>
#include <stdbool.h>

#define MAX_FRAMES 10
#define MAX_PAGES 50


bool isPageInMemory(int memory[], int frameSize, int page) {
    for (int i = 0; i < frameSize; i++) {
        if (memory[i] == page) {
            return true;
        }
    }
    return false;
}

void printMemoryState(int step, int page, int memory[], int frameSize, bool pageFault) {
    if (step == 1) {
        printf("Step | Page | Memory State         | Page Fault?\n");
        printf("--------------------------------------------------\n");
    }

    printf("%-4d | %-4d | [", step, page);
    for (int i = 0; i < frameSize; i++) {
        if (memory[i] == -1) {
            printf("-");
        } else {
            printf("%d", memory[i]);
        }
        if (i < frameSize - 1) {
            printf(", ");
        }
    }
    printf("]               | %s\n", pageFault ? "Yes" : "No");
}

void fifo(int pages[], int pageCount, int frameSize) {
    int memory[MAX_FRAMES];
    int pageFaults = 0;
    int front = 0; 
    for (int i = 0; i < frameSize; i++) {
        memory[i] = -1;
    }
    printf("--- FIFO Algorithm ---\n");
    for (int step = 0; step < pageCount; step++) {
        bool pageFault = !isPageInMemory(memory, frameSize, pages[step]);
        if (pageFault) {
            memory[front] = pages[step]; 
            front = (front + 1) % frameSize; 
            pageFaults++;
        }
        printMemoryState(step + 1, pages[step], memory, frameSize, pageFault);
    }
    printf("Total Page Faults: %d\n", pageFaults);
}
void lru(int pages[], int pageCount, int frameSize) {
    int memory[MAX_FRAMES];
    int recent[MAX_FRAMES]; 
    int pageFaults = 0;


    for (int i = 0; i < frameSize; i++) {
        memory[i] = -1;
        recent[i] = -1;
    }

    printf("--- LRU Algorithm ---\n");

    for (int step = 0; step < pageCount; step++) {
        bool pageFault = !isPageInMemory(memory, frameSize, pages[step]);

        if (pageFault) {
            int lruIndex = 0;
            for (int i = 1; i < frameSize; i++) {
                if (recent[i] < recent[lruIndex]) {
                    lruIndex = i;
                }
            }

            memory[lruIndex] = pages[step];
            recent[lruIndex] = step;     
            pageFaults++;
        } else {

            for (int i = 0; i < frameSize; i++) {
                if (memory[i] == pages[step]) {
                    recent[i] = step;
                    break;
                }
            }
        }

        printMemoryState(step + 1, pages[step], memory, frameSize, pageFault);
    }

    printf("Total Page Faults: %d\n", pageFaults);
}


int main() {
    int pages[MAX_PAGES];
    int frameSize, pageCount;


    printf("Enter the number of frames (max %d): ", MAX_FRAMES);
    scanf("%d", &frameSize);

    if (frameSize > MAX_FRAMES || frameSize <= 0) {
        printf("Error: Frame size must be between 1 and %d.\n", MAX_FRAMES);
        return 1;
    }
    printf("Enter the number of pages: ");
    scanf("%d", &pageCount);

    if (pageCount > MAX_PAGES || pageCount <= 0) {
        printf("Error: Page count must be between 1 and %d.\n", MAX_PAGES);
        return 1;
    }

    printf("Enter the page reference sequence:\n");
    for (int i = 0; i < pageCount; i++) {
        printf("%d: ", i + 1);
        scanf("%d", &pages[i]);
    }

    printf("\nFIFO:\n");
    fifo(pages, pageCount, frameSize);

    printf("\nLRU:\n");
    lru(pages, pageCount, frameSize);

    return 0;
}
