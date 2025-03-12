using System;

class Minesweeper
{
    const int width = 5;
    const int height = 8;
    const int UNFLAGGED_MINE = -1;
    const int FLAGGED_MINE = -2;
    const int INCORRECT_FLAGGED_MINE = -3;
    const int UNKNOWN = -4;
    const int num_mine = 10;

    static int[,] mineField = new int[height, width];
    static int numIncorrectFlaggedMine = 0;
    static int numCorrectFlaggedMine = 0;
    static bool explode = false;

    static char GetTag(int c, bool explode)
    {
        switch (c)
        {
            case UNKNOWN: return '-';
            case UNFLAGGED_MINE: return explode ? '*' : '-';
            case FLAGGED_MINE: return '!';
            case INCORRECT_FLAGGED_MINE: return explode ? '&' : '!';
            default: return (char)(c + '0');
        }
    }

    static void DisplayField(bool explode)
    {
        Console.WriteLine("   0 1 2 3 4\n");
        for (int i = 0; i < height; i++)
        {
            Console.Write($"{i}  ");
            for (int j = 0; j < width; j++)
            {
                Console.Write($"{GetTag(mineField[i, j], explode)} ");
            }
            Console.WriteLine();
        }
    }

    static void SetCell(int i, int j, int value)
    {
        mineField[i, j] = value;
    }

    static int GetCell(int i, int j)
    {
        return mineField[i, j];
    }

    static void SetMine(int numOfMine)
    {
        int currentMine = 0;
        Random rand = new Random();
        while (currentMine < numOfMine)
        {
            int i = rand.Next(height);
            int j = rand.Next(width);
            if (GetCell(i, j) != UNFLAGGED_MINE)
            {
                SetCell(i, j, UNFLAGGED_MINE);
                currentMine++;
            }
        }
    }

    static void InitField(int numOfMine)
    {
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                SetCell(i, j, UNKNOWN);
            }
        }
        SetMine(numOfMine);
    }

    static bool IsMine(int i, int j)
    {
        int cell = GetCell(i, j);
        return cell == UNFLAGGED_MINE || cell == FLAGGED_MINE;
    }

    static int Count(int i, int j)
    {
        int count = 0;
        for (int a = i - 1; a <= i + 1; a++)
        {
            for (int b = j - 1; b <= j + 1; b++)
            {
                if (a == i && b == j) continue;
                if (a >= 0 && a < height && b >= 0 && b < width)
                {
                    if (IsMine(a, b)) count++;
                }
            }
        }
        return count;
    }

    static void Reveal(int i, int j)
    {
        if (IsMine(i, j))
        {
            explode = true;
        }
        else
        {
            SetCell(i, j, Count(i, j));
        }
    }

    static void ExecuteCmd(char action, int row, int col)
    {
        int i = row;
        int j = col;
        switch (action)
        {
            case 'f':
                if (IsMine(i, j))
                {
                    SetCell(i, j, FLAGGED_MINE);
                    numCorrectFlaggedMine++;
                }
                else
                {
                    SetCell(i, j, INCORRECT_FLAGGED_MINE);
                    numIncorrectFlaggedMine++;
                }
                break;
            case 'r':
                Reveal(i, j);
                break;
            case 'u':
                if (GetCell(i, j) == FLAGGED_MINE)
                {
                    SetCell(i, j, UNFLAGGED_MINE);
                    numCorrectFlaggedMine--;
                }
                else if (GetCell(i, j) == INCORRECT_FLAGGED_MINE)
                {
                    SetCell(i, j, UNFLAGGED_MINE);
                    numIncorrectFlaggedMine--;
                }
                break;
        }
    }

    static void Main()
    {
        InitField(num_mine);
        DisplayField(false);

        while (!explode && (numCorrectFlaggedMine < num_mine || numIncorrectFlaggedMine > 0))
        {
            Console.WriteLine("Enter cmd:");
            string input = Console.ReadLine();
            if (input.Length < 3) continue;

            char cmd = input[0];
            int row = input[1] - '0';
            int col = input[2] - '0';

            ExecuteCmd(cmd, row, col);
            DisplayField(explode);
            Console.WriteLine($"Number of mine left: {num_mine - (numCorrectFlaggedMine + numIncorrectFlaggedMine)}");
        }

        Console.WriteLine(explode ? ":(" : ":)");
    }
}
