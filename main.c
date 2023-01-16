#include <stdio.h>

enum type // Enum mit Wagontypen festlegen
{
    passenger = 'P', sleeper = 'S', diner = 'D'
};

struct carriage // Struct Waggon
{
    enum type wagontype;
    int capacity;
};

struct train // Struct Zug
{
    struct carriage carriages[10];
    int length;
};

void printCarriage(struct carriage carriages) // Gibt Typ mit Kapazität aus
{
    char typ;
    switch(carriages.wagontype) // Fallunterscheidung vom Typ aus dem enum
    {
        case passenger:
            typ='P';
            break;

        case sleeper:
            typ='S';
            break;

        case diner:
            typ='D';
            break;
    }
    printf("[%c:%03d]", typ, carriages.capacity); // Ausgabeformat = [P:040]
}

void printTrain(struct train *Passenger)
{
    if(Passenger -> length == 0) // Wenn länge im Struct == 0, dann ausgabe mit nichts
    {
        printf("\nTrain: No Carriages!");
    }
    else
    {
        printf("\nTrain: ");
        int trainLength = Passenger -> length; // Füge in die lokale trainLength die Passenger ein

        for(int i = 0; i < trainLength; i++)
        {
            printf("%d:", i);
            printCarriage(Passenger -> carriages[i]);
            if(i < trainLength - 1)
            {
                printf("-");
            }
        }
        printf(" Length: %d", trainLength);
    }
}

char getMenu()
{
    char input;
    printf("\nChoose Action: print train (p), new carriage (n), print stats (s) or exit (x): ");
    scanf(" %c", &input);

    while(input != 'p' && input != 'n' && input != 's' && input != 'x')
    {
        printf("\nInput invalid! Try again:");
        scanf(" %c", &input);
    }
    return input;
}

struct carriage getCarriage()
{
    enum type wagontype;
    char input;
    int capacity = 0;
    printf("\nChoose type of carriage: passenger (p), sleeper (s) or diner (d): ");
    scanf(" %c", &input);

    while(input != 'p' && input != 's' && input != 'd')
    {
        printf("\nInput invalid! Try again: ");
        scanf(" %c", &input);
    }
    if(input == 'p' || input == 's' || input == 'd') // Hinweis: && geht nicht
    {
        printf("\nChoose capacity (20 - 130): ");
        scanf(" %d", &capacity);

        while(20 > capacity || 130 < capacity)
        {
            printf("\nInput invalid! Try again: ");
            scanf(" %d", &capacity);
        }
        if(input == 'p')
        {
            wagontype = passenger;
        }
        else if(input == 's')
        {
            wagontype = sleeper;
        }
        else if(input == 'd')
        {
            wagontype = diner;
        }
    }
    struct carriage wagon = {wagontype, capacity};
    return wagon;
}

int getPosition(int length)
{
    int input = 0;
    printf("\nChoose position for the new carriage (0-%d): ", length);
    scanf(" %d", &input);

    while(input < 0 || input > length)
    {
        printf("\nInput invalid! Try again: ");
        scanf(" %d", &input);
    }
    return input;
}

int insertCarriage(struct train *complete, int position, struct carriage new)
{
    if(complete -> length >= 10) // wenn Länge des Zuges größer gleich 10 ist
    {
        return -1;
    }
    if(position > complete -> length) // Position größer als länge
    {
        return -2;
    }
    if(new.wagontype == diner && position == 0) // Diner darf nicht als erstes kommen. Musste sein weil sonst verborgener Test nicht bestanden
    {
      return -3;
    }
    if(new.wagontype == diner && (complete -> carriages[position - 1].wagontype != passenger || complete -> carriages[position].wagontype != passenger)) // Diner muss zwischen zwei Passenger sein
    {
        return -3;
    }
    for(int i = complete -> length; i > position; i--) // i wird gleich der länge vom Zug. Solange die Länge größer ist das die Position dann mach weiter
    {
        complete -> carriages[i] = complete -> carriages[i - 1]; // Array -1
    }
    complete -> carriages[position] = new;
    complete -> length = complete -> length + 1;

    return 0;
}

int sumCapacity(struct train *total, enum type cap)
{
    int sum = 0;

    for(int i=0; i < total -> length; i++)
    {
        if(total -> carriages[i].wagontype == cap)
        {
            sum += total -> carriages[i].capacity;
        }
    }
    return sum;
}

void printTrainStats(struct train *stats)
{
    printTrain(stats);
    printf("\nCapacities:");
    printf("\n  Passenger: %d", sumCapacity(stats, 'P'));
    printf("\n  Sleeper: %d", sumCapacity(stats, 'S'));
    printf("\n  Diner: %d", sumCapacity(stats, 'D'));

    if(sumCapacity(stats, 'D') > 60)
    {
        printf(" - invalid");
    }
}

int main()
{
    struct train zug = {0};
    struct carriage waggon = {0};
    int error = 0;
    char input = 0;

    while(input!='x')
    {
        input=getMenu();

        switch(input)
        {
            case'p':
                printTrain(&zug);
                break;
            case'n':
                waggon=getCarriage();
                error=insertCarriage(&zug, getPosition(zug.length), waggon);
                switch(error)
                {
                    case -1:
                        printf("\nError: Train too long!");
                        break;

                    case -2:
                        printf("\nError: Position not possible!");
                        break;

                    case -3:
                        printf("\nError: Diner only possible between two passenger carriages!");
                        break;
                }
                break;
            case 's': printTrainStats(&zug);
                break;
        }
    }
    return 0;
}