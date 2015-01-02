# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('counting_app', '0001_initial'),
    ]

    operations = [
        migrations.DeleteModel(
            name='DataCounter',
        ),
        migrations.DeleteModel(
            name='ResultsCounter',
        ),
        migrations.AddField(
            model_name='job',
            name='cpu_time',
            field=models.FloatField(default=0),
            preserve_default=True,
        ),
    ]
