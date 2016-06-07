/*
 * ASoC Driver for RPi-PCM1803.
 *
 * Author:	Vladimir Davydov <davydov.vladimir@gmail.com>
 *		Copyright 2015
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 */

#include <linux/module.h>
#include <linux/platform_device.h>

#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include <sound/jack.h>

static int snd_rpi_rpi_pcm1803_init(struct snd_soc_pcm_runtime *rtd)
{
	return 0;
}

static int snd_rpi_rpi_pcm1803_hw_params(struct snd_pcm_substream *substream,
				       struct snd_pcm_hw_params *params)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;

	return snd_soc_dai_set_bclk_ratio(cpu_dai, 32*2);
}

/* machine stream operations */
static struct snd_soc_ops snd_rpi_rpi_pcm1803_ops = {
	.hw_params = snd_rpi_rpi_pcm1803_hw_params,
};

static struct snd_soc_dai_link snd_rpi_rpi_pcm1803_dai[] = {
{
	.name		= "RPi-PCM1803",
	.stream_name	= "RPi-PCM1803 HiFi",
#if 0
	.cpu_dai_name	= "bcm2708-i2s.0",
	.codec_dai_name	= "pcm1803",
	.platform_name	= "bcm2708-i2s.0",
#else
	.cpu_dai_name	= "3f203000.i2s",//magic name!!! How kernel generate it? Byt device tree?
	.codec_dai_name	= "pcm1803",
	.platform_name	= "3f203000.i2s",
#endif
	.codec_name	= "soc:pcm1803-codec",
	.dai_fmt	= SND_SOC_DAIFMT_I2S | SND_SOC_DAIFMT_NB_NF |
				SND_SOC_DAIFMT_CBM_CFM,
	.ops		= &snd_rpi_rpi_pcm1803_ops,
	.init		= snd_rpi_rpi_pcm1803_init,
},
};

/* audio machine driver */
static struct snd_soc_card snd_rpi_rpi_pcm1803 = {
	.name         = "snd_rpi_rpi_pcm1803",
	.dai_link     = snd_rpi_rpi_pcm1803_dai,
	.num_links    = ARRAY_SIZE(snd_rpi_rpi_pcm1803_dai),
};

static int snd_rpi_rpi_pcm1803_probe(struct platform_device *pdev)
{
	int ret = 0;

	printk(KERN_ERR"snd_rpi_rpi_pcm1803_probe\n");
	snd_rpi_rpi_pcm1803.dev = &pdev->dev;
	ret = snd_soc_register_card(&snd_rpi_rpi_pcm1803);
	if (ret)
		dev_err(&pdev->dev, "snd_soc_register_card() failed: %d\n", ret);

	return ret;
}

static int snd_rpi_rpi_pcm1803_remove(struct platform_device *pdev)
{
	return snd_soc_unregister_card(&snd_rpi_rpi_pcm1803);
}

static const struct of_device_id snd_rpi_rpi_pcm1803_of_match[] = {
	{ .compatible = "rpi,rpi-pcm1803", },
	{},
};
MODULE_DEVICE_TABLE(of, snd_rpi_rpi_pcm1803_of_match);

static struct platform_driver snd_rpi_rpi_pcm1803_driver = {
        .driver = {
                .name   = "snd-rpi-pcm1803",
                .owner  = THIS_MODULE,
                .of_match_table = snd_rpi_rpi_pcm1803_of_match,
        },
        .probe          = snd_rpi_rpi_pcm1803_probe,
        .remove         = snd_rpi_rpi_pcm1803_remove,
};

module_platform_driver(snd_rpi_rpi_pcm1803_driver);

MODULE_AUTHOR("Vladimir Davydov <davydov.vladimir@gmail.com>");
MODULE_DESCRIPTION("ASoC Driver for RPi-PCM1803");
MODULE_LICENSE("GPL v2");
