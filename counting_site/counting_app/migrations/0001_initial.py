# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
    ]

    operations = [
        migrations.CreateModel(
            name='DataCounter',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('created_jobs', models.IntegerField()),
                ('created_ids', models.IntegerField()),
                ('allocated_ids', models.IntegerField()),
                ('reported_ids', models.IntegerField()),
                ('date_started', models.DateTimeField(auto_now_add=True)),
                ('date_last', models.DateTimeField()),
                ('cpu_time', models.FloatField()),
            ],
            options={
            },
            bases=(models.Model,),
        ),
        migrations.CreateModel(
            name='Job',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('secret_hash', models.CharField(max_length=64)),
                ('n', models.IntegerField()),
                ('n0', models.IntegerField()),
                ('low_id', models.IntegerField()),
                ('hight_id', models.IntegerField()),
                ('date_created', models.DateTimeField(auto_now_add=True)),
                ('date_allocated', models.DateTimeField()),
                ('date_reported', models.DateTimeField()),
                ('ip_allocated', models.CharField(max_length=15)),
            ],
            options={
            },
            bases=(models.Model,),
        ),
        migrations.CreateModel(
            name='JobResult',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('size', models.IntegerField()),
                ('result', models.IntegerField()),
                ('job', models.ForeignKey(to='counting_app.Job')),
            ],
            options={
            },
            bases=(models.Model,),
        ),
        migrations.CreateModel(
            name='ResultsCounter',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('size', models.IntegerField()),
                ('result', models.IntegerField()),
            ],
            options={
            },
            bases=(models.Model,),
        ),
    ]
