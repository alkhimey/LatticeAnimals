# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('counting_app', '0012_auto_20150913_1516'),
    ]

    operations = [
        migrations.AlterField(
            model_name='keyvalueresult',
            name='value',
            field=models.CharField(max_length=32),
            preserve_default=True,
        ),
    ]
