# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('counting_app', '0004_auto_20141227_1353'),
    ]

    operations = [
        migrations.RenameField(
            model_name='resultcounter',
            old_name='result',
            new_name='accamulator',
        ),
    ]
