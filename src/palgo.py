import ctypes as ct
from collections import defaultdict

def get_array(l: [int]) -> ct.Array[ct.c_int]:
    return (ct.c_int * len(l))(*l)

def get_arrays(l1: [int], l2: [int]) -> (ct.Array[ct.c_int], ct.Array[ct.c_int], int):
    assert len(l1) == len(l2)
    return (get_array(l1), get_array(l2), len(l1))

class Record:
    def __init__(self, g, m, s):
        self.g = g
        self.m = m
        self.s = s

    def __repr__(self) -> str:
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
    exercises_wrapper = palgo.exercises_wrapper
    exercises_wrapper.restype = PalgoRecordList
    maxmg_arr, minw_arr, size = get_arrays(maxmg, minw)
    result = exercises_wrapper(3, maxmg_arr, minw_arr, size, mins, maxs)
    records = result.to_record_list()
    palgo.free_wrapper(result)
    return records

class Muscle:
    def __init__(self, name, max_daily, min_weekly):
        self.name       = name
        self.max_daily  = max_daily
        self.min_weekly = min_weekly

    def __repr__(self) -> str:
        return "(\"{}\", {}, {})".format(self.name, self.max_daily, self.min_weekly)

class NamedRecord:
    def __init__(self, name, sets):
        self.name = name
        self.sets = sets

    def __repr__(self) -> str:
        return "(\"{}\", {})".format(self.name, self.sets)

class Schedule:
    def __init__(self, days: [[NamedRecord]]):
        self.days = days

    def __repr__(self) -> str:
        result = ""
        for day, exercises in enumerate(self.days):
            for exercise in exercises:
                result += str(day) + " " + str(exercise) + "\n"
        return result

def palgo_schedule(palgo, ndays, muscles: [Muscle], mins=3, maxs=4) -> Schedule:
    max_daily  = [muscle.max_daily  for muscle in muscles]
    min_weekly = [muscle.min_weekly for muscle in muscles]
    exercises = palgo_exercises(palgo, ndays, max_daily, min_weekly, mins, maxs)
    days = [[] for _ in range(ndays)]
    for exercise in exercises:
        days[exercise.g].append(
                NamedRecord(muscles[exercise.m].name, exercise.s))

    return Schedule(days)

def schedule_to_muscles(source: Schedule) -> [Muscle]:
    names      = set()
    max_daily  = defaultdict(int)
    min_weekly = defaultdict(int)

    for day in source.days:
        sets   = defaultdict(int)
        for exercise in day:
            sets[exercise.name] += exercise.sets
            names.add(exercise.name)
        for k, v in sets.items():
            max_daily[k]   = max(max_daily[k], v)
            min_weekly[k] += v

    muscles = []
    for name in names:
        muscles.append(Muscle(name, max_daily[name], min_weekly[name]))

    return muscles

def palgo_reformat(palgo, schedule: Schedule) -> Schedule:
    muscles  = schedule_to_muscles(schedule)
    schedule = palgo_schedule(palgo, len(schedule.days), muscles)
    return schedule
