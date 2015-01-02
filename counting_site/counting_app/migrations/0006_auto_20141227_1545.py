# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('counting_app', '0005_auto_20141227_1538'),
    ]

    operations = [
        migrations.RenameField(
            model_name='job',
            old_name='hight_id',
            new_name='high_id',
        ),
    ]
