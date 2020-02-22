# Flask Web Demo

Contained in the directory are files which will run a test site which access a sqlite3 database through python. The site can be ran using two different methods: locally or though Docker

## Set up

To host this site on a local machine, the following commands will need to be ran to access the site in the browser:
```bash
#Command line

# if downloaded though zip, ignore this step
git clone https:github.com/lnestelroad/CTTOOLS-33.git

cd CTTOOLS-33

flask run
```

or 

```bash
#Docker
docker build -t lnestelroad/flask-demo .
docker run -d -p 5000:5000 lnestelroad/flask-demo
```

Then simply go to http://localhost:5000 to access the test site.
