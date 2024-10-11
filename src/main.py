import ctypes as ct

from palgo import (
    Muscle,
    Schedule,
    NamedRecord,
    palgo_reformat,
    palgo_schedule,
)

palgo = ct.CDLL("libpalgo.so")

def test0():
    days = 3;
    muscles = [
        Muscle("Petto",     11, 22),
        Muscle("Schiena",   30, 20),
        Muscle("Spalle",    8,  20),
        Muscle("Gambe",     20, 20),
        Muscle("Bicipiti",  10, 23),
        Muscle("Tricipiti", 8,  16),
    ];
    schedule = palgo_schedule(palgo, days, muscles)
    print("TEST1", schedule, sep='\n')

def test1():
    days = [
        [
            NamedRecord("Schiena", 5),
            NamedRecord("Schiena", 3),
            NamedRecord("Schiena", 4),
            NamedRecord("Schiena", 4),
            NamedRecord("Schiena", 4),
            NamedRecord("Tricipiti", 4),
            NamedRecord("Tricipiti", 4),
            NamedRecord("Spalle", 4),
        ], [
            NamedRecord("Petto", 4),
            NamedRecord("Petto", 4),
            NamedRecord("Petto", 3),
            NamedRecord("Bicipiti", 3),
            NamedRecord("Bicipiti", 4),
            NamedRecord("Bicipiti", 3),
            NamedRecord("Spalle", 4),
        ], [
            NamedRecord("Gambe", 4),
            NamedRecord("Gambe", 4),
            NamedRecord("Gambe", 4),
            NamedRecord("Gambe", 4),
            NamedRecord("Spalle", 4),
            NamedRecord("Spalle", 4),
            NamedRecord("Bicipiti", 3),
        ], [
            NamedRecord("Petto", 4),
            NamedRecord("Petto", 4),
            NamedRecord("Petto", 3),
            NamedRecord("Bicipiti", 3),
            NamedRecord("Tricipiti", 4),
            NamedRecord("Bicipiti", 4),
            NamedRecord("Tricipiti", 4),
            NamedRecord("Bicipiti", 3),
            NamedRecord("Spalle", 4),
        ]
    ]
    schedule = palgo_reformat(palgo, Schedule(days))
    print("TEST2", schedule, sep='\n')


if __name__ == "__main__":
    test0()
    test1()
