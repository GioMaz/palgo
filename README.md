# Palgo

Algoritmo per schede di palestra vincolate

### Funzionamento

L'api richiede il passaggio di un json nel body contenente i vincoli.
Un esempio è il seguente:

```bash
{
    "days": 3,
        "muscles": [
        {
            "name": "Petto",
            "maxmg": 12,
            "minw": 24
        },
        {
            "name":"Schiena",
            "maxmg":30,
            "minw":24
        },
        {
            "name":"Spalle",
            "maxmg":8,
            "minw":24},
        {
            "name":"Gambe",
            "maxmg":12,
            "minw":12
        },
        {
            "name":"Bicipiti",
            "maxmg":12,
            "minw":24
        },
        {
            "name":"Tricipiti",
            "maxmg":8,
            "minw":12
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
