# Palgo

Algorithm for the creation of constrained training schedules.

For the explaination of the underlying algorithm read the [documentation](./docs/main.pdf).

### Building

First compile and execute the backend

```bash
cd src/palgo
make palgo_server
./palgo_server
```

Then run the web interface

```bash
cd src/webapi
npm i
npm run start
```

### How it works

To apply the algorithm pass a json string containing the input constraints to the body of the request:

```json
{
    "days": 3,
        "muscles": [
        {
            "name": "Upper body",
            "maxDaily": 4,
            "minWeekly": 10
        },
        {
            "name":"Lower body",
            "maxDaily": 3,
            "minWeekly": 8
        }
    ]
}

```

An exmaple of a request is the following:

```bash
curl http://localhost:8081/api/new \
     -H 'Content-Type: application/json' \
     -X POST \
     -v \
     -d @body.json
```

The result should be something like this:

```json
[
  [
    { "name": "Upper body", "sets": 3 },
    { "name": "Lower body", "sets": 3 }
  ],
  [
    { "name": "Upper body", "sets": 4 },
    { "name": "Lower body", "sets": 2 }
  ],
  [
    { "name": "Upper body", "sets": 3 },
    { "name": "Lower body", "sets": 3 }
  ]
]
```
