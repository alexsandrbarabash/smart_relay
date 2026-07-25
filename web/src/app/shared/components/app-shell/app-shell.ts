import { Component, input } from '@angular/core';

@Component({
  selector: 'app-shell',
  standalone: true,
  templateUrl: './app-shell.html',
  styleUrl: './app-shell.scss',
})
export class AppShell {
  readonly title = input.required<string>();
  readonly eyebrow = input<string>('Time');
  readonly deviceTimeLabel = input<string>('');
}
