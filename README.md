# Palgo

Algorithm for the creation of constrained training schedules.

For the explaination of the underlying algorithm read the [documentation](./docs/main.pdf).

### Funzionamento

To apply the algorithm pass a json string containing the input constraints to the body of the request:

```json
{
    "days": 3,
        "muscles": [
        {
            "name": "Upper body",
            "maxDaily": 12,
            "minWeekly": 24
        },
        {
            "name":"Lower body",
            "maxDaily": 30,
            "minWeekly": 24
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
