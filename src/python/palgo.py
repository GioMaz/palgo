import ctypes as ct

def get_array(l: [int]) -> ct.Array[ct.c_int]:
    return (ct.c_int * len(l))(*l)

def get_arrays(l1: [int], l2: [int]) -> (ct.Array[ct.c_int],
                                         ct.Array[ct.c_int], int):
    if len(l1) != len(l2):
        return None
    return (get_array(l1), get_array(l2), len(l1))

class Record:
    def __init__(self, g, m, s):
        self.g = g
        self.m = m
        self.s = s

    def __str__(self) -> str:
        return "(g{}, m{}, s{})".format(self.g, self.m, self.s)

class PalgoRecord(ct.Structure):
    _fields_ = [("g", ct.c_int),
                ("m", ct.c_int),
                ("s", ct.c_int)]

    def to_record(self) -> Record:
        return Record(self.g, self.m, self.s)

class PalgoRecordList(ct.Structure):
    _fields_ = [("records", ct.POINTER(PalgoRecord)),
                ("size", ct.c_int)]

    def to_record_list(self) -> [Record]:
        record_list = []
        for i in range(self.size):
            record_list.append(self.records[i].to_record())
        return record_list

def palgo_exercises(palgo, N, maxmg, minw, mins=3, maxs=4) -> [Record]:
    palgo_exercises_wrapper = palgo.exercises_wrapper
    palgo_exercises_wrapper.restype = PalgoRecordList
    maxmg_arr, minw_arr, size = get_arrays(maxmg, minw)
    result = palgo_exercises_wrapper(3, maxmg_arr, minw_arr, size, mins, maxs)
    exercises = result.to_record_list()
    palgo.free_wrapper(result)
    return exercises

class Muscle:
    def __init__(self, name, max_daily, min_weekly):
        self.name       = name
        self.max_daily  = max_daily
        self.min_weekly = min_weekly

class NamedRecord:
    def __init__(self, name, sets):
        self.name = name
        self.sets = sets

    def __str__(self) -> str:
        return "(\"{}\", {})".format(self.name, self.sets)

def palgo_named_records(palgo, n: int, muscles: [Muscle], mins=3, maxs=4) -> [NamedRecord]:
    max_daily  = []
    min_weekly = []
    for muscle in muscles:
        max_daily.append(muscle.max_daily)
        min_weekly.append(muscle.min_weekly)
    exercises = palgo_exercises(palgo, n, max_daily, min_weekly,
                                mins, maxs)
    days = [[]] * n
    for exercise in exercises:
        days[exercise.g].append(
            NamedRecord(muscles[exercise.m].name, exercise.s))
    return days
