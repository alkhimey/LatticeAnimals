# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('counting_app', '0010_auto_20150103_0126'),
    ]

    operations = [
        migrations.CreateModel(
            name='KeyValueResult',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('key', models.CharField(max_length=32)),
                ('value', models.CharField(max_length=128)),
                ('job', models.ForeignKey(to='counting_app.Job')),
            ],
            options={
            },
            bases=(models.Model,),
        ),
        migrations.RemoveField(
            model_name='resultcounter',
            name='config',
        ),
        migrations.DeleteModel(
            name='ResultCounter',
        ),
    ]
