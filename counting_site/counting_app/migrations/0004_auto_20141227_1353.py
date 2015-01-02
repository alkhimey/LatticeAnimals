# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('counting_app', '0003_auto_20141227_0146'),
    ]

    operations = [
        migrations.CreateModel(
            name='ResultCounter',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('size', models.IntegerField()),
                ('result', models.IntegerField(default=0)),
            ],
            options={
            },
            bases=(models.Model,),
        ),
        migrations.RemoveField(
            model_name='jobresult',
            name='job',
        ),
        migrations.DeleteModel(
            name='JobResult',
        ),
        migrations.AddField(
            model_name='job',
            name='results',
            field=models.CharField(max_length=1024, null=True),
            preserve_default=True,
        ),
    ]
