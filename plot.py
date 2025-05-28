import os
import sys
import glob
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import matplotlib.image as mpimg

sns.set(style="whitegrid")


class BenchmarkPlotter:
  def __init__(self, csv_path):
    self.csv_path = csv_path
    self.name = os.path.splitext(os.path.basename(csv_path))[0]
    self.df = self._load_data()
    self.plot_files = []
    self.output_dir = f"plots/{self.name}"
    os.makedirs(self.output_dir, exist_ok=True)

  def _load_data(self):
    try:
      df = pd.read_csv(self.csv_path)
      if df.empty:
        raise ValueError("CSV is empty")
      df["SizeLabel"] = df["Size"].apply(lambda x: f"{int(x):,}")
      return df
    except Exception as e:
      print(f"❌ Failed to load '{self.csv_path}': {e}")
      sys.exit(1)

  def generate_plots(self):
    unique_functions = self.df["Function"].unique()
    unique_types = self.df["Type"].unique()

    for func in unique_functions:
      safe_func = func.replace(':', '_').replace('/', '_')
      func_dir = os.path.join(self.output_dir, safe_func)
      os.makedirs(func_dir, exist_ok=True)

      fig, axes = plt.subplots(
        1, len(unique_types),
        figsize=(6 * len(unique_types), 5),
        sharey=True
      )
      if len(unique_types) == 1:
        axes = [axes]

      for ax, typ in zip(axes, unique_types):
        subset = self.df[(self.df["Function"] == func) & (self.df["Type"] == typ)]
        sns.lineplot(
          data=subset,
          x="SizeLabel",
          y="Time",
          hue="Namespace",
          style="Namespace",
          markers=True,
          dashes=False,
          ax=ax
        )
        ax.set_title(typ)
        ax.set_xlabel("Size")
        if ax == axes[0]:
          ax.set_ylabel("Time (s)")
        else:
          ax.set_ylabel("")
        ax.legend(title="Namespace")

      fig.suptitle(f"{self.name}::{func}()", fontsize=16)
      plt.tight_layout(rect=[0, 0.03, 1, 0.95])
      fname = os.path.join(func_dir, f"{safe_func}.png")
      plt.savefig(fname)
      self.plot_files.append(fname)
      plt.close()

    print(f"✅ Plots saved for '{self.name}'")


class Slideshow:
  def __init__(self, image_files):
    self.files = image_files
    self.index = 0
    self.fig, self.ax = plt.subplots(figsize=(12, 7))
    self.fig.canvas.mpl_connect("key_press_event", self.on_key)
    self.show_image(0)

  def show_image(self, i):
    self.ax.clear()
    self.ax.axis("off")
    try:
      img = mpimg.imread(self.files[i])
      self.ax.imshow(img)
      self.fig.suptitle(f"{os.path.basename(self.files[i])}  ({i + 1}/{len(self.files)})", fontsize=14)
      self.fig.canvas.draw_idle()
    except Exception as e:
      print(f"❌ Error loading image: {self.files[i]}\n{e}")
      plt.close()

  def on_key(self, event):
    if event.key == "right":
      self.index = (self.index + 1) % len(self.files)
      self.show_image(self.index)
    elif event.key == "left":
      self.index = (self.index - 1) % len(self.files)
      self.show_image(self.index)
    elif event.key == "escape":
      plt.close()

  def start(self):
    plt.axis("off")
    plt.show()


def main():
  csv_files = glob.glob("benchmark_*.csv")
  if not csv_files:
    print("❌ No benchmark_*.csv files found.")
    sys.exit(1)

  all_images = []

  for csv_file in csv_files:
    plotter = BenchmarkPlotter(csv_file)
    plotter.generate_plots()
    all_images.extend(plotter.plot_files)

  if all_images:
    print("📽️ Launching slideshow...")
    slideshow = Slideshow(all_images)
    slideshow.start()
  else:
    print("⚠️ No plots generated.")


if __name__ == "__main__":
  main()
