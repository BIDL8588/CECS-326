#include <stdio.h>
#include <stdbool.h>

#define MAX_FRAMES 10
#define MAX_PAGES 50


bool isPageInMemory(int memory[], int frameSize, int page) {// memory holds frames 
    for (int i = 0; i < frameSize; i++) {// number of memory slots being used 
        if (memory[i] == page) {// compares each element with target value page number 
            return true;// true if value exists 
        }
    }
    return false;
}

void printMemoryState(int step, int page, int memory[], int frameSize, bool pageFault) {// step number, page being processed, array of memory frames, number of memory frames, boolean whether page fault occurred. 
    if (step == 1) {
        printf("Step | Page | Memory State         | Page Fault?\n");
        printf("--------------------------------------------------\n");
    }

    printf("%-4d | %-4d | [", step, page);//table header for first step
    for (int i = 0; i < frameSize; i++) {// prints the current step and page 
        if (memory[i] == -1) {//iterates through memory slots 
            printf("-");
        } else {
            printf("%d", memory[i]);
        }
        if (i < frameSize - 1) {// if empty slots then print - 
            printf(", ");
        }
    }
    printf("]               | %s\n", pageFault ? "Yes" : "No");
}

void fifo(int pages[], int pageCount, int frameSize) {// page requests, total number of pages in the sequence, number of memory frames 
    int memory[MAX_FRAMES];// memory slots 
    int pageFaults = 0;// total page faults 
    int front = 0; // records the position of the oldest page to replace FIFO 
    for (int i = 0; i < frameSize; i++) {
        memory[i] = -1;// initialize memory to -1. 
    }
    printf("--- FIFO Algorithm ---\n");// header 
    for (int step = 0; step < pageCount; step++) {// iterates through all pages in the sequence 
        bool pageFault = !isPageInMemory(memory, frameSize, pages[step]);// checks the current page that exists in memory
        if (pageFault) {
            memory[front] = pages[step]; 
            front = (front + 1) % frameSize; 
            pageFaults++;
        }//if page fault occurs then the page will be placed in the fron position
        printMemoryState(step + 1, pages[step], memory, frameSize, pageFault);
    }
    printf("Total Page Faults: %d\n", pageFaults);// print total number of page faults after processing all pages 
}
void lru(int pages[], int pageCount, int frameSize) {
    int memory[MAX_FRAMES];
    int recent[MAX_FRAMES]; 
    int pageFaults = 0;// same as fifo function 


    for (int i = 0; i < frameSize; i++) {// memory represents the memory slots 
        memory[i] = -1;
        recent[i] = -1;// array to track each memory, both memory and recent set to empty 
    }

    printf("--- LRU Algorithm ---\n");//header 

    for (int step = 0; step < pageCount; step++) {//iterates through all pages in the sequence 
        bool pageFault = !isPageInMemory(memory, frameSize, pages[step]);

        if (pageFault) {// checks if the current page is in memory 
            int lruIndex = 0;
            for (int i = 1; i < frameSize; i++) {
                if (recent[i] < recent[lruIndex]) {
                    lruIndex = i;
                }
            }

            memory[lruIndex] = pages[step]; // if a page fault occurrs then the process will find the least recently used page 
            recent[lruIndex] = step; //smallest value represents the least recently used slot 
            pageFaults++;
        } else {//replace the page at the LRU index and update the access time in recent

            for (int i = 0; i < frameSize; i++) {
                if (memory[i] == pages[step]) {
                    recent[i] = step;
                    break;
                }
            }
        }
        // if the page is in memory then the process will update the access time in recent to the current step 
        printMemoryState(step + 1, pages[step], memory, frameSize, pageFault);
    }

    printf("Total Page Faults: %d\n", pageFaults);//total number of page faults 
}

// Main function to get user input, reference sequence, number of pages, and frame size
int main() {
    int pages[MAX_PAGES];
    int frameSize, pageCount;


    printf("Enter the number of frames (max %d): ", MAX_FRAMES);// frame size 
    scanf("%d", &frameSize);

    if (frameSize > MAX_FRAMES || frameSize <= 0) {
        printf("Error: Frame size must be between 1 and %d.\n", MAX_FRAMES);
        return 1;
    }
    printf("Enter the number of pages: ");//pages 
    scanf("%d", &pageCount);

    if (pageCount > MAX_PAGES || pageCount <= 0) {//validates number of max pages 
        printf("Error: Page count must be between 1 and %d.\n", MAX_PAGES);
        return 1;
    }

    printf("Enter the page reference sequence:\n");
    for (int i = 0; i < pageCount; i++) {
        printf("%d: ", i + 1);
        scanf("%d", &pages[i]);
    }

    printf("\nFIFO:\n");//FIFO function runs 
    fifo(pages, pageCount, frameSize);

    printf("\nLRU:\n");//LRU function runs
    lru(pages, pageCount, frameSize);

    return 0;
}

