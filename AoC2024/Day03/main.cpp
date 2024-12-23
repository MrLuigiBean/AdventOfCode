#include <iostream>
#include <fstream>

#define STAGES_SIZE 8
const char USE_NUMBERS = '#';

void Reset(int* stageIndex, int* firstNum, int* secondNum)
{
    *stageIndex = 0;
    *firstNum = 0;
    *secondNum = 0;
}

void Process(int* total, int* firstNum, int* secondNum)
{
    *total += *firstNum * *secondNum;
}

int main_01()
{
    std::ifstream file("input.txt", std::ios_base::in);

    char stages[] = { {'m'}, {'u'}, {'l'}, {'('},{ USE_NUMBERS }, {','}, { USE_NUMBERS }, {')'} };
    int stageIndex = 0;
    int firstNum = 0;
    int secondNum = 0;
    int totalSum = 0;

    char ch = 0;
    while ((ch = file.get()) >= 0)
    {
        char currentStage = stages[stageIndex];
        bool isUsingNumbers = currentStage == USE_NUMBERS;

        if (isUsingNumbers)
        {
            bool isFirstNumber = stageIndex < 5; // sorry for the hack...

            bool shouldCollectDigits = true;

            // Terminate digit collection if current char belongs to the next stage.
            if (ch == stages[stageIndex + 1])
            {
                currentStage = stages[++stageIndex]; // advance current stage
                isUsingNumbers = false; // updated due to stage advancing
                shouldCollectDigits = false;
            }

            if (shouldCollectDigits)
            {
                if (!isdigit(ch))
                {
                    Reset(&stageIndex, &firstNum, &secondNum);
                    continue;
                }

                if (isFirstNumber)
                    firstNum = firstNum * 10 + (ch - '0');
                else
                    secondNum = secondNum * 10 + (ch - '0');
            }
        }

        if (!isUsingNumbers)
        {
            if (ch != currentStage)
            {
                Reset(&stageIndex, &firstNum, &secondNum);
                continue;
            }
            else // if (ch == currentStage)
            {
                ++stageIndex;

                if (stageIndex >= STAGES_SIZE)
                {
                    Process(&totalSum, &firstNum, &secondNum);
                    Reset(&stageIndex, &firstNum, &secondNum);
                }
            }
        }
    }

    printf("Grand total: %d\n", totalSum);
    return 0;
}

int main_02()
{
    return 0;
}

int main()
{
    // main_01();
    main_02();
    return 0;
}
