# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('counting_app', '0011_auto_20150912_0013'),
    ]

    operations = [
        migrations.AlterField(
            model_name='config',
            name='max_id',
            field=models.IntegerField(help_text=b'Total Number of policubes of size n0 without applying filter. Determined from algo id and n0'),
        ),
    ]
