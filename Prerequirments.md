

## Server


* Django 1.9.4

* sudo apt-get install libmysqlclient-dev
* (2.7) sudo pip install MySQL-python
* (3.x) pip install mysqlclient

* ssh -L 3306:clown.mysql.pythonanywhere-services.com:3306 clown@ssh.pythonanywhere.com

* python manage.py makemigrations counting_app

* python manage.py migrate

* python manage.py createsuperuser

* python manage.py collectstatic


