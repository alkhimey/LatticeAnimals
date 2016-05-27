

## Server


* Django 1.9.4
* sudo apt-get install libmysqlclient-dev
* sudo pip install MySQL-python


* ssh -L 3306:clown.mysql.pythonanywhere-services.com:3306 clown@ssh.pythonanywhere.com

* python manage.py makemigrations counting_app

* python manage.py migrate

* python manage.py createsuperuser 


