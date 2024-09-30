const convertU32ToU8ArrayLE = n => {
  return [
    (n >> 0)  & 255,
    (n >> 8)  & 255,
    (n >> 16) & 255,
    (n >> 24) & 255,
  ];
}

const convertU32ArrayToU8ArrayLE = array => {
  return array.reduce((acc, val) => {
    return acc.concat(convertU32ToU8ArrayLE(val))
  }, []);
}

const convertU8ArrayToU32LE = array => {
  return (array[0] << 0)
       | (array[1] << 8)
       | (array[2] << 16)
       | (array[3] << 24);
}

const convertU8ArrayToU32ArrayLE = array => {
  const result = [];
  for (let i = 0; i < array.length; i += 4) {
    const n = convertU8ArrayToU32LE(array.slice(i, i+4));
    result.push(n);
  }
  return result;
}

class Record {
  constructor(g, m, s) {
    this.g = g;
    this.m = m;
    this.s = s;
  }
}

const palgoAlgo = async (N, maxmg, minw) => {
  const net = require('net');

  const resultU8  = [];

  const socket = new net.Socket();

  socket.connect({ host: 'localhost', port: 8080 }, () => {
    const NU8     = convertU32ArrayToU8ArrayLE([N]);
    const maxmgU8 = convertU32ArrayToU8ArrayLE([maxmg.length].concat(maxmg));
    const minwU8  = convertU32ArrayToU8ArrayLE([minw.length].concat(minw));

    const NArray      = new Uint8Array(NU8);
    const maxmgArray  = new Uint8Array(maxmgU8);
    const minwArray   = new Uint8Array(minwU8);

    socket.write(NArray);
    socket.write(maxmgArray);
    socket.write(minwArray);
  });

  socket.on('data', data => {
    resultU8.push(...data);
  });

  return new Promise(resolve => {
    socket.on('close', () => {
      const result = convertU8ArrayToU32ArrayLE(resultU8);
      const exercises = [];
      for (let i = 0; i < result[0]; i++) {
        exercises.push(new Record(
          result[3*i+1],
          result[3*i+2],
          result[3*i+3],
        ));
      }
      resolve(exercises);
    });
  })
};

class NamedRecord {
  constructor(name, sets) {
    this.name = name;
    this.sets = sets;
  }
}

class Muscle {
  constructor(name, maxmg, minw) {
    this.name   = name;
    this.maxmg  = maxmg;
    this.minw   = minw;
  }
};

const wrapper = async (days, muscles) => {
  const maxmg = [];
  const minw  = [];
  muscles.forEach(muscle => {
    maxmg.push(muscle.maxmg);
    minw.push(muscle.minw);
  });
  const records = await palgoAlgo(days, maxmg, minw);
  if (records.length == 0) {
    throw new Error("Input constraints are not satisfiable.");
  }

  const namedRecords = Array(days).fill().map(_ => []);
  records.forEach(record => {
    namedRecords[record.g].push(
      new NamedRecord(muscles[record.m].name, record.s)
    );
  });
  return namedRecords;
};

const test = async () => {
  const days = 3;
  const muscles = [
    new Muscle("Petto",     11, 22),
    new Muscle("Schiena",   30, 20),
    new Muscle("Spalle",    8,  20),
    new Muscle("Gambe",     20, 20),
    new Muscle("Bicipiti",  10, 23),
    new Muscle("Tricipiti", 8,  16),
  ];

  try {
    const exercises = await wrapper(days, muscles);
    console.log(exercises);
  } catch (error) {
    console.log(error)
  }
};

(async () => {
  await test();
})();

module.exports = { NamedRecord, wrapper, Muscle };
