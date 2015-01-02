# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('counting_app', '0007_auto_20150101_2200'),
    ]

    operations = [
        migrations.AlterField(
            model_name='config',
            name='algo_id',
            field=models.CharField(help_text=b'Which counting algorithm to tell the client to use', max_length=8),
            preserve_default=True,
        ),
        migrations.AlterField(
            model_name='config',
            name='secret_hash_length',
            field=models.IntegerField(default=32, help_text=b'Max is 64'),
            preserve_default=True,
        ),
    ]
