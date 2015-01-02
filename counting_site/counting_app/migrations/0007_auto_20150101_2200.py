# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('counting_app', '0006_auto_20141227_1545'),
    ]

    operations = [
        migrations.CreateModel(
            name='Config',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('date_created', models.DateTimeField(auto_now_add=True)),
                ('date_activated', models.DateTimeField(auto_now_add=True)),
                ('client_version', models.IntegerField(help_text=b'Which client version this config can communicate with')),
                ('algo_id', models.IntegerField(help_text=b'Which counting algorithm to tell the client to use')),
                ('n', models.IntegerField(help_text=b'The size of the polyocubes we want to count')),
                ('n0', models.IntegerField(help_text=b'At which size we want to split the counting')),
                ('max_id', models.IntegerField(help_text=b'Total Number of policubes of size n0 without applying predicate. Determined from algo id and n0')),
                ('num_of_jobs', models.IntegerField(help_text=b'To how many jobs we want to split the counting')),
                ('secret_hash_length', models.IntegerField(default=32)),
                ('minutes_before_realloc', models.IntegerField(default=10, help_text=b'Minutes to wait before an unreported job can be reallocated')),
            ],
            options={
            },
            bases=(models.Model,),
        ),
        migrations.RemoveField(
            model_name='job',
            name='date_created',
        ),
        migrations.RemoveField(
            model_name='job',
            name='n',
        ),
        migrations.RemoveField(
            model_name='job',
            name='n0',
        ),
        migrations.AddField(
            model_name='job',
            name='config',
            field=models.ForeignKey(default=1, to='counting_app.Config'),
            preserve_default=False,
        ),
        migrations.AddField(
            model_name='resultcounter',
            name='config',
            field=models.ForeignKey(default=1, to='counting_app.Config'),
            preserve_default=False,
        ),
    ]
