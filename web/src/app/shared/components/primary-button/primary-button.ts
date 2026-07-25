import { Component, input } from '@angular/core';
import { LucideSave } from '@lucide/angular';

@Component({
  selector: 'app-primary-button',
  standalone: true,
  imports: [LucideSave],
  templateUrl: './primary-button.html',
  styleUrl: './primary-button.scss',
})
export class PrimaryButton {
  readonly disabled = input(false);
  readonly loading = input(false);
  readonly type = input<'button' | 'submit'>('button');
}
