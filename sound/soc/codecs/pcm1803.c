/*
 * pcm1803.c  --  SoC audio for PCM1803 chip
 *
 * Author: Liam Girdwood <lrg@slimlogic.co.uk>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 */

#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/soc.h>
#include <sound/soc-dapm.h>

static struct snd_soc_dai_driver pcm1803_dai = {
	.name = "pcm1803",
	.capture = {
		.stream_name = "Capture",
		.channels_min = 2,
		.channels_max = 2,
		.rates = SNDRV_PCM_RATE_CONTINUOUS,
		.formats = SNDRV_PCM_FMTBIT_S32_LE | SNDRV_PCM_FMTBIT_S24_LE | SNDRV_PCM_FMTBIT_U32_BE | SNDRV_PCM_FMTBIT_S24_BE | SNDRV_PCM_FMTBIT_U24_BE,
//		.formats =  SNDRV_PCM_FMTBIT_U32_BE |  SNDRV_PCM_FMTBIT_U24_BE,
	},
};

static const struct snd_soc_dapm_widget pcm1803_dapm_widgets[] = {
	SND_SOC_DAPM_AIF_OUT("pcm1803 AIF", "Capture", 0,
			     SND_SOC_NOPM, 0, 0),
	SND_SOC_DAPM_INPUT("pcm1803"),
};

static const struct snd_soc_dapm_route intercon[] = {
	{"pcm1803 AIF", NULL, "pcm1803"},
};

static struct snd_soc_codec_driver soc_pcm1803 = {
	.dapm_widgets = pcm1803_dapm_widgets,
	.num_dapm_widgets = ARRAY_SIZE(pcm1803_dapm_widgets),
	.dapm_routes = intercon,
	.num_dapm_routes = ARRAY_SIZE(intercon),
};

static int pcm1803_dev_probe(struct platform_device *pdev)
{
	printk(KERN_ERR"pcm1803_dev_probe\n");
	return snd_soc_register_codec(&pdev->dev,
			&soc_pcm1803, &pcm1803_dai, 1);
}

static int pcm1803_dev_remove(struct platform_device *pdev)
{
	snd_soc_unregister_codec(&pdev->dev);
	return 0;
}

static const struct of_device_id pcm1803_of_match[] = { 
	{ .compatible = "ti,pcm1803", }, 
	{ } 
}; 
MODULE_DEVICE_TABLE(of, pcm1803_of_match);

MODULE_ALIAS("platform:pcm1803-codec");

static struct platform_driver pcm1803_driver = {
	.driver = {
		.name = "pcm1803-codec",
		.owner = THIS_MODULE,
		 .of_match_table = of_match_ptr(pcm1803_of_match),
	},
	.probe = pcm1803_dev_probe,
	.remove = pcm1803_dev_remove,
};

module_platform_driver(pcm1803_driver);

MODULE_DESCRIPTION("PCM1803 driver");
MODULE_AUTHOR("Vladimir Davydov <davydov.vladimir@gmail.com>");
MODULE_LICENSE("GPL");
