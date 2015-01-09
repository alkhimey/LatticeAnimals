# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('counting_app', '0009_auto_20150102_1429'),
    ]

    operations = [
        migrations.AlterField(
            model_name='config',
            name='date_activated',
            field=models.DateTimeField(null=True),
            preserve_default=True,
        ),
    ]
