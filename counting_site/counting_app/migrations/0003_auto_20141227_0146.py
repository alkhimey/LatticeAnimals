# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('counting_app', '0002_auto_20141223_1453'),
    ]

    operations = [
        migrations.AlterField(
            model_name='job',
            name='date_allocated',
            field=models.DateTimeField(null=True),
            preserve_default=True,
        ),
        migrations.AlterField(
            model_name='job',
            name='date_reported',
            field=models.DateTimeField(null=True),
            preserve_default=True,
        ),
        migrations.AlterField(
            model_name='job',
            name='ip_allocated',
            field=models.CharField(max_length=15, null=True),
            preserve_default=True,
        ),
    ]
