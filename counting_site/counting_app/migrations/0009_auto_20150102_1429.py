# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('counting_app', '0008_auto_20150101_2205'),
    ]

    operations = [
        migrations.AlterField(
            model_name='config',
            name='algo_id',
            field=models.IntegerField(help_text=b'Which counting algorithm to tell the client to use'),
            preserve_default=True,
        ),
        migrations.AlterField(
            model_name='config',
            name='client_version',
            field=models.CharField(help_text=b'Which client version this config can communicate with', max_length=8),
            preserve_default=True,
        ),
    ]
