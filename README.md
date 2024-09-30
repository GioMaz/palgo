# Palgo

Algoritmo per la creazione di schede di palestra vincolate.

Per la spiegazione del funzionamento dell'algoritmo alla base consultare la [documentazione](./documentation/main.pdf)

### Funzionamento

L'api richiede il passaggio di un json nel body contenente i vincoli come da esempio:

```bash
{
    "days": 3,
        "muscles": [
        {
            "name": "Upper body",
            "maxmg": 12,
            "minw": 24
        },
        {
            "name":"Lower body",
            "maxmg":30,
            "minw":24
        }
    ]
}
```

Un esempio di richiesta è la seguente:

```bash
curl http://localhost:8081/api/new \
     -H 'Content-Type: application/json' \
     -X POST \
     -v \
     -d @body.json
```
